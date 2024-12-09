// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/Storages/ISStorageMaster.h"

#include "Character/ISCharacter.h"
#include "Game/ISPlayerController.h"


AISStorageMaster::AISStorageMaster()
{
	StorageContainer = CreateDefaultSubobject<UISStorageContainer>("Storage Container");
	StorageContainer->SetIsReplicated(true);
	bReplicates = true;
	bNetLoadOnClient = true;
}

void AISStorageMaster::BeginPlay()
{
	StorageContainer->InitializeContainer();
	check(StorageUserWidgetClass);
	StorageWidget = CreateWidget<UISMenuUIBase>(GetWorld(),StorageUserWidgetClass);  //创建UI
	StorageWidget->SetWidgetOwner(this);
}

void AISStorageMaster::OnBuildingWasInteract_Implementation(const AActor* InteractingActor,
	const UActorComponent* InteractingComponent)
{
	APlayerController* SourcePC = Cast<APlayerController>( InteractingActor->GetInstigatorController());
	if(SourcePC)
	{
		StorageUIOpen(SourcePC);
	}
}


void AISStorageMaster::StorageUIOpen(APlayerController* TargetController)
{
	check(StorageWidget);
	AISPlayerController* SourcePC = Cast<AISPlayerController>(TargetController);  //转换成本地PC
	if(!SourcePC) return;
	SourcePC->OnOpenInventoryEvent.AddDynamic(this,&AISStorageMaster::StorageUIClose);
	if(!StorageWidget->IsVisible())
	{
		AddOwnerController(TargetController);

		SourcePC->bStorageUIOpen = true;
		StorageWidget->InitializeCraftingData(StorageContainer);
		StorageWidget->AddToViewport();
		SourcePC->bShowMouseCursor = true;
		SourcePC->SetInputMode(FInputModeGameAndUI());
		SourcePC->InputSubsystem->RemoveMappingContext(SourcePC->CharacterInputMapping);
		SourcePC->InputSubsystem->AddMappingContext(SourcePC->CharacterInputMenuMapping,0);
	}
}

void AISStorageMaster::StorageUIClose(APlayerController* TargetController)
{
	AISPlayerController* SourcePC = Cast<AISPlayerController>(TargetController);
	if(!SourcePC) return;
	SourcePC->OnOpenInventoryEvent.RemoveDynamic(this,&AISStorageMaster::StorageUIClose);
	if(StorageWidget->IsVisible())
	{
		RemoveOwnerController(TargetController);
		
		SourcePC->bStorageUIOpen = false;
		StorageWidget->RemoveFromParent();
		SourcePC->bShowMouseCursor = false;
		SourcePC->SetInputMode(FInputModeGameOnly());
		SourcePC->InputSubsystem->RemoveMappingContext(SourcePC->CharacterInputMenuMapping);
		SourcePC->InputSubsystem->AddMappingContext(SourcePC->CharacterInputMapping,0);
	}
}

void AISStorageMaster::AddOwnerController_Implementation(APlayerController* TargetController)
{
	SetOwner(TargetController);
	StorageContainer->AddRelevantPlayer(TargetController);
}

void AISStorageMaster::RemoveOwnerController_Implementation(APlayerController* TargetController)
{
	SetOwner(nullptr);
	StorageContainer->RemoveRelevantPlayer(TargetController);
}
