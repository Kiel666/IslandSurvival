// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/ISItemsContainer.h"
#include "ISCharacterInventory.generated.h"
/**
 * 
 */
UCLASS()
class ISLANDSURVIVAL_API UISCharacterInventory : public UISItemsContainer
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FWhenItemPickUp ItemPickup;
protected:
	virtual void BeginPlay() override;
};
