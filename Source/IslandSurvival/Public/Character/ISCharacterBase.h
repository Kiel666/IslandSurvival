// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "ISCharacterBase.generated.h"

//初始化结构体
USTRUCT()
struct FCharacterParams
{
	GENERATED_BODY()
	FCharacterParams(){}
	FCharacterParams(UAbilitySystemComponent*ASC,UAttributeSet*AS,APlayerController*PC,APlayerState*PS):SourceASC(ASC),SourceAS(AS),SourcePC(PC),SourcePS(PS){}
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>SourceASC = nullptr;
	UPROPERTY()
	TObjectPtr<UAttributeSet> SourceAS = nullptr;
	UPROPERTY()
	TObjectPtr<APlayerController>SourcePC = nullptr;
	UPROPERTY()
	TObjectPtr<APlayerState> SourcePS = nullptr;
};
UCLASS()
class ISLANDSURVIVAL_API AISCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AISCharacterBase();
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};