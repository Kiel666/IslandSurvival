// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ISPlayerMainHUD.h"



UISMainUIWidgetController* AISPlayerMainHUD::GetMainUIWidgetController(const FCharacterParams& CharacterParams)
{
	if(IsMainUIWidgetController==nullptr)
	{
		IsMainUIWidgetController = NewObject<UISMainUIWidgetController>(GetWorld(),ISMainUIWidgetControllerClass);
		IsMainUIWidgetController->SetWidgetControllerParams(CharacterParams);
		IsMainUIWidgetController->BindCallBackDependencies();
	}
	return IsMainUIWidgetController;
}

UISMenuWidgetController* AISPlayerMainHUD::GetMenuWidgetController(const FCharacterParams& CharacterParams)
{
	if(IsMenuWidgetController==nullptr)
	{
		IsMenuWidgetController = NewObject<UISMenuWidgetController>(GetWorld(),ISMenuWidgetControllerClass);
		IsMenuWidgetController->SetWidgetControllerParams(CharacterParams);
		IsMenuWidgetController->BindCallBackDependencies();
	}
	return IsMenuWidgetController;
}

void AISPlayerMainHUD::InitUserWidget(const FCharacterParams& CharacterParams)
{
	UISMainUIWidgetController* MainUIWidgetController = GetMainUIWidgetController(CharacterParams);
	UISMenuWidgetController* MenuWidgetController = GetMenuWidgetController(CharacterParams);
	IsMainUI = CreateWidget<UISMainUIBase>(GetWorld(),ISMainUIClass);
	ISMenuUI = CreateWidget<UISMenuUIBase>(GetWorld(),ISMenuClass); //创建人物的背包界面UI
	IsMainUI->SetWidgetController(MainUIWidgetController);
	ISMenuUI->SetWidgetController(MenuWidgetController);
	MainUIWidgetController->BroadcastInitialValues();  //广播初始化属性
	MenuWidgetController->BroadcastInitialValues();  //广播初始化属性
	IsMainUI->AddToViewport();
}

