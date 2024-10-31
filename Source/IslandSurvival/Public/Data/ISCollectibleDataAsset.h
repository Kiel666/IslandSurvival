// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ISPlayerItemDataTable.h"
#include "Engine/DataAsset.h"
#include "ISCollectibleDataAsset.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ECollectibleClass : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	Tree = 1 UMETA(DisplayName = "Tree"),
};
USTRUCT(BlueprintType)
struct FDropConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName TargetName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat DropRate = FScalableFloat();  //掉落几率，和角色等级有关
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FScalableFloat DropNums = FScalableFloat();  //掉落数量，与角色等级有关(暂定)
};
USTRUCT(BlueprintType)
struct FDropInformation
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FDropConfig> Drops;
};
UCLASS()
class ISLANDSURVIVAL_API UISCollectibleDataAsset : public UDataAsset
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly)
	TMap<ECollectibleClass, FDropInformation> DropConfigs;
	FDropInformation GetDropConfig(ECollectibleClass TargetClass);
};
