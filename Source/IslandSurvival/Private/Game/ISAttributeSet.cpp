// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ISAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Interface/ISCombatInterface.h"
#include "Interface/ISPlayerInterface.h"
#include "Net/UnrealNetwork.h"

UISAttributeSet::UISAttributeSet()
{
	InitMaxHealthLevel(1.f);
}

void UISAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UISAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UISAttributeSet,Attack,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UISAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UISAttributeSet,Vigor,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UISAttributeSet,MaxVigor,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UISAttributeSet,WeaponAttack,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UISAttributeSet,GatheringDamage,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UISAttributeSet,Hunger,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UISAttributeSet,MaxHunger,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UISAttributeSet,Thirst,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UISAttributeSet,MaxThirst,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UISAttributeSet,PlayerLevel,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UISAttributeSet,MaxHealthLevel,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UISAttributeSet,Armor,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UISAttributeSet,GearArmor,COND_None,REPNOTIFY_Always);
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
	FEffectProperties Properties;
	SetEffectContext(Data, Properties);
	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(),0.f,GetMaxHealth()));
	}
	if(Data.EvaluatedData.Attribute == GetHungerAttribute())
	{
		SetHunger(FMath::Clamp(GetHunger(),0.f,GetMaxHunger()));
	}
	if(Data.EvaluatedData.Attribute == GetThirstAttribute())
	{
		SetThirst(FMath::Clamp(GetThirst(),0.f,GetMaxThirst()));
	}
	if(Data.EvaluatedData.Attribute == GetVigorAttribute())
	{
		SetVigor(FMath::Clamp(GetVigor(),0.f,GetMaxVigor()));
	}
	if(Data.EvaluatedData.Attribute == GetInComingXPAttribute())
	{
		const float LocalInComingXP = GetInComingXP();  //保存当前得到的经验值
		SetInComingXP(0.f);  //重置为0

		//若角色可以访问角色接口和战斗接口
		if(Properties.SourceCharacter->Implements<UISPlayerInterface>()&&Properties.SourceCharacter->Implements<UISCombatInterface>())
		{
			const int32 CurrentLevel = IISPlayerInterface::Execute_GetLevel(Properties.SourceCharacter);  //获得当前等级
			const int32 CurrentXP = IISPlayerInterface::Execute_GetXP(Properties.SourceCharacter); //获得当前经验值

			const int32 NewLevel = IISPlayerInterface::Execute_FindLevelFromXP(Properties.SourceCharacter, CurrentXP+LocalInComingXP);  //新加的经验值加上已有的经验值然后查询等级
			const int32 NewLevelUp = NewLevel - CurrentLevel;
			//如果当前等级大于获取到的等级，则已升级，满足升级条件
			if(NewLevelUp>0)
			{
				int32 AttributePointReward = IISPlayerInterface::Execute_GetAttributePointsReward(Properties.SourceCharacter,NewLevelUp);  //获取升级后的属性点
				IISPlayerInterface::Execute_AddToPlayerLevel(Properties.SourceCharacter,NewLevelUp);  //添加等级
				const float ResultValue = IISPlayerInterface::Execute_GetLevel(Properties.SourceCharacter);
				SetPlayerLevel(FMath::Clamp(ResultValue,0.f,100.f));
				IISPlayerInterface::Execute_AddToAttributePoints(Properties.SourceCharacter,AttributePointReward);  //累加属性点
				IISPlayerInterface::Execute_LevelUp(Properties.SourceCharacter);  //升级
			}
			IISPlayerInterface::Execute_AddToXP(Properties.SourceCharacter, LocalInComingXP); //添加加上的经验值
		}
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

void UISAttributeSet::OnRep_WeaponAttack(const FGameplayAttributeData& OldWeaponAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UISAttributeSet,WeaponAttack,OldWeaponAttack);  //武器伤害
}

void UISAttributeSet::OnRep_GatheringDamage(const FGameplayAttributeData& OldGatheringDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UISAttributeSet,GatheringDamage,OldGatheringDamage);
}

void UISAttributeSet::OnRep_Hunger(const FGameplayAttributeData& OldHunger) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UISAttributeSet,Hunger,OldHunger);
}

void UISAttributeSet::OnRep_MaxHunger(const FGameplayAttributeData& OldMaxHunger) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UISAttributeSet,MaxHunger,OldMaxHunger);
}

void UISAttributeSet::OnRep_Thirst(const FGameplayAttributeData& OldThirst) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UISAttributeSet,Thirst,OldThirst);
}

void UISAttributeSet::OnRep_MaxThirst(const FGameplayAttributeData& OldMaxThirst) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UISAttributeSet,MaxThirst,OldMaxThirst);
}

void UISAttributeSet::OnRep_PlayerLevel(const FGameplayAttributeData& OldPlayerLevel) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UISAttributeSet,PlayerLevel,OldPlayerLevel);

}

void UISAttributeSet::OnRep_MaxHealthLevel(const FGameplayAttributeData& OldMaxHealthLevel) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UISAttributeSet,MaxHealthLevel,OldMaxHealthLevel);
}

void UISAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UISAttributeSet,Armor,OldArmor);
}

void UISAttributeSet::OnRep_GearArmor(const FGameplayAttributeData& OldGearArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UISAttributeSet,GearArmor,OldGearArmor);
}
