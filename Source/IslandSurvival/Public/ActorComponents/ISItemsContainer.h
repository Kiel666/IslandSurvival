// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ISPlayerItemDataTable.h"
#include "Interface/ISContainerInterface.h"
#include "ISItemsContainer.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWhenItemPickUp,FItemInformation,TargetItemInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemPickUpOnUI,FItemInformation,TargetItemInfo);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ISLANDSURVIVAL_API UISItemsContainer : public UActorComponent,public IISContainerInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ContainerName;  //背包名字
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 InventorySpace = 0;  //背包空间
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 LastChooseIndex = -1;
	FItemInformation ItemInfo;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EContainerType ContainerType = EContainerType::None;
	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FOnInventoryUpdate InventoryUpdate;
	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FWhenItemPickUp ItemPickup;
	UPROPERTY(BlueprintAssignable,BlueprintReadWrite)
	FItemPickUpOnUI ItemPickupOnUI;
	UISItemsContainer();
	void WhenInventoryChange(UISItemsContainer*TargetContainer,const int32 TargetIndex);
	UFUNCTION(BlueprintCallable,Client,Reliable)
	void WhenItemExchanged(UISItemsContainer*TargetItemsContainer,const int32 SourceIndex,const int32 TargetIndex);
	UFUNCTION(Client,Reliable)
	void ToPickUpItemsInBackPack(const FItemInformation Information);  //拾取物品函数，在客户端上运行
	UFUNCTION()
	void InitializeBackPackSpace(const int32 Space);
	bool CheckInventoryEmpty(const FItemInformation Information);
	UPROPERTY(BlueprintReadOnly,Replicated)
	TArray<FItemInformation>InventoryContainer;//背包
protected:
	virtual void BeginPlay() override;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual EContainerType GetTargetContainerType() override;
	virtual FName GetContainerName() override;
	void PickUpItemForActor(APawn* TargetPawn, AActor* TargetActor);
	UFUNCTION(Client,Reliable)
	void PickUpItemForID(APawn*TargetPawn,FName TargetID,const int32 TargetNums);

};
