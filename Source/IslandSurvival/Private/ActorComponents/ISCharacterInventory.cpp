// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ISCharacterInventory.h"
void UISCharacterInventory::BeginPlay()
{
	Super::BeginPlay();
	if(GetOwner()->HasAuthority())
	{
		InitializeBackPackSpace(InventorySpace);  //初始化背包
	}
	ItemPickup.AddDynamic(this,&UISItemsContainer::ToPickUpItemsInBackPack);
}
