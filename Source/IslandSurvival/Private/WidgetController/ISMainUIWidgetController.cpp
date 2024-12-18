// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetController/ISMainUIWidgetController.h"

void UISMainUIWidgetController::BindCallBackDependencies()
{
	Super::BindCallBackDependencies();

	
	ISAbilitySystem->GetGameplayAttributeValueChangeDelegate(GetSourceAttributeSet()->GetHealthAttribute()).AddLambda([this]
		(const FOnAttributeChangeData& Data)
	{

		OnHealthChange.Broadcast(Data.NewValue);
	});
	ISAbilitySystem->GetGameplayAttributeValueChangeDelegate(GetSourceAttributeSet()->GetMaxHealthAttribute()).AddLambda([this]
	(const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChange.Broadcast(Data.NewValue);
	});
	ISAbilitySystem->GetGameplayAttributeValueChangeDelegate(GetSourceAttributeSet()->GetVigorAttribute()).AddLambda([this]
	(const FOnAttributeChangeData& Data)
	{
		OnVigorChange.Broadcast(Data.NewValue);
	});
	ISAbilitySystem->GetGameplayAttributeValueChangeDelegate(GetSourceAttributeSet()->GetMaxVigorAttribute()).AddLambda([this]
	(const FOnAttributeChangeData& Data)
	{
		OnMaxVigorChange.Broadcast(Data.NewValue);
	});
	ISAbilitySystem->GetGameplayAttributeValueChangeDelegate(GetSourceAttributeSet()->GetHungerAttribute()).AddLambda([this]
	(const FOnAttributeChangeData& Data)
	{
		OnHungerChange.Broadcast(Data.NewValue);
	});
	ISAbilitySystem->GetGameplayAttributeValueChangeDelegate(GetSourceAttributeSet()->GetMaxHungerAttribute()).AddLambda([this]
	(const FOnAttributeChangeData& Data)
	{
		OnMaxHungerChange.Broadcast(Data.NewValue);
	});
	ISAbilitySystem->GetGameplayAttributeValueChangeDelegate(GetSourceAttributeSet()->GetThirstAttribute()).AddLambda([this]
	(const FOnAttributeChangeData& Data)
	{
		OnThirstChange.Broadcast(Data.NewValue);
	});
	ISAbilitySystem->GetGameplayAttributeValueChangeDelegate(GetSourceAttributeSet()->GetMaxThirstAttribute()).AddLambda([this]
	(const FOnAttributeChangeData& Data)
	{
		OnMaxThirstChange.Broadcast(Data.NewValue);
	});
}

void UISMainUIWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	OnMaxHealthChange.Broadcast(GetSourceAttributeSet()->GetMaxHealth());
	OnHealthChange.Broadcast(GetSourceAttributeSet()->GetHealth());
	OnMaxVigorChange.Broadcast(GetSourceAttributeSet()->GetMaxVigor());
	OnVigorChange.Broadcast(GetSourceAttributeSet()->GetVigor());
	OnMaxHungerChange.Broadcast(GetSourceAttributeSet()->GetMaxHunger());
	OnHungerChange.Broadcast(GetSourceAttributeSet()->GetHunger());
	OnMaxThirstChange.Broadcast(GetSourceAttributeSet()->GetMaxThirst());
	OnThirstChange.Broadcast(GetSourceAttributeSet()->GetThirst());
}
