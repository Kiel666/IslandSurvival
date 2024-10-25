// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISInteractionComponent.h"

#include "Character/ISCharacter.h"
#include "Interface/ISItemInterface.h"

// Sets default values for this component's properties
UISInteractionComponent::UISInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UISInteractionComponent::PrimaryIntract()
{
	AISCharacter*ISCharacter = Cast<AISCharacter>(GetOwner());
	if(!ISCharacter) return;
	UCameraComponent*SourceCamera = ISCharacter->CameraComponent;
	FVector EyeLocation = SourceCamera->GetComponentLocation();
	FVector EyeForwardLocation = SourceCamera->GetForwardVector();
	FVector End = EyeLocation+(EyeForwardLocation*120);

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit,EyeLocation,End,ECC_GameTraceChannel1);  //待添加一个自定义通道
	DrawDebugLine(GetWorld(), EyeLocation, Hit.GetActor() ? Hit.Location : End, FColor::Red, false, 1.0f);

	AActor*HitActor = Hit.GetActor();
	if(HitActor)
	{
		if(HitActor->Implements<UISItemInterface>())
		{
			APawn*MyPawn = Cast<APawn>(GetOwner());
			IISItemInterface::Execute_PickUpItemToInventory(HitActor,MyPawn,HitActor);
		}
	}
	
}

void UISInteractionComponent::TickInteractline()
{
	AISCharacter*ISCharacter = Cast<AISCharacter>(GetOwner());
	if(!ISCharacter) return;
	UCameraComponent*SourceCamera = ISCharacter->CameraComponent;
	FVector EyeLocation = SourceCamera->GetComponentLocation();
	FVector EyeForwardLocation = SourceCamera->GetForwardVector();
	FVector End = EyeLocation+(EyeForwardLocation*120);

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit,EyeLocation,End,ECC_GameTraceChannel1);  //待添加一个自定义通道
	//DrawDebugLine(GetWorld(), EyeLocation, Hit.GetActor() ? Hit.Location : End, FColor::Green, false, 1.0f);

	AActor*HitActor = Hit.GetActor();
	if(HitActor)
	{
		if(HitActor->Implements<UISItemInterface>())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("拾取"));
		}
	}
}

void UISInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}
void UISInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TickInteractline();
}

