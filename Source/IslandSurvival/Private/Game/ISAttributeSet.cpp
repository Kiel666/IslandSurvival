// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ISAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

void UISAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UISAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UISAttributeSet,Attack,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UISAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UISAttributeSet,Vigor,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UISAttributeSet,MaxVigor,COND_None,REPNOTIFY_Always);
}



void UISAttributeSet::SetEffectContext(const FGameplayEffectModCallbackData& Data, FEffectProperties& Properties)
{
	Properties.EffectContextHandle = Data.EffectSpec.GetContext();
	Properties.SourceASC = Properties.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();  //返回始作俑者的ASC

	if(IsValid(Properties.SourceASC) && Properties.SourceASC->AbilityActorInfo.IsValid()&&Properties.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Properties.SourceAvatarActor = Properties.SourceASC->AbilityActorInfo->AvatarActor.Get();//获取对象Actor
		Properties.SourceController = Properties.SourceASC->AbilityActorInfo->PlayerController.Get(); //获取对象控制器
		//如果控制器存在但是对象Actor不存在
		if(Properties.SourceController==nullptr&&Properties.SourceAvatarActor!=nullptr)
		{
			if(const APawn*Pawn = Cast<APawn>(Properties.SourceAvatarActor))
			{
				Properties.SourceController = Pawn->GetController();
			}
		}
		if(Properties.SourceController)
		{
			Properties.SourceCharacter = Cast<ACharacter>(Properties.SourceController->GetPawn());
		}
	}
	if(Data.Target.AbilityActorInfo.IsValid()&&Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Properties.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Properties.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Properties.TargetCharacter = Cast<ACharacter>(Properties.TargetAvatarActor);  //将目标Actor转换成角色
		Properties.TargetASC = &Data.Target;
	}
}

void UISAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		
	}
}

void UISAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if(Attribute==GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxHealth());
	}
	else if(Attribute==GetVigorAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxVigor());
	}
}



void UISAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UISAttributeSet,Health,OldHealth);
}

void UISAttributeSet::OnRep_Attack(const FGameplayAttributeData& OldAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UISAttributeSet,Attack,OldAttack);
}

void UISAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UISAttributeSet,MaxHealth,OldMaxHealth);
}

void UISAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UISAttributeSet,Vigor,OldVigor);
}

void UISAttributeSet::OnRep_MaxVigor(const FGameplayAttributeData& OldMaxVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UISAttributeSet,MaxVigor,OldMaxVigor);
}