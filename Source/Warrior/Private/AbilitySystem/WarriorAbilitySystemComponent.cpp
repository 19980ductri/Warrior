// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WarriorAbilitySystemComponent.h"

#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "DataAssets/DataAsset_HeroStartupData.h"
#include "DataAssets/DataAsset_StartupDataBase.h"


UWarriorAbilitySystemComponent::UWarriorAbilitySystemComponent()
{
	
}

void UWarriorAbilitySystemComponent::GrantAbilities(const TArray<TSubclassOf<UWarriorGameplayAbility>>& AbilitiesToGrant,
                                                    const int32 Level)
{
	if (AbilitiesToGrant.Num() <= 0) return;

	for (const auto& Ability : AbilitiesToGrant)
	{
		if (!Ability)
		{
			continue;
		}
		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = Level;
		GiveAbility(AbilitySpec);
	}
}

void UWarriorAbilitySystemComponent::ApplyGameplayEffect(
	const TArray<TSubclassOf<UGameplayEffect>> StartupGameplayEffect, int Level)
{
	if (StartupGameplayEffect.Num() <= 0) return;
	for (const auto& EffectClass : StartupGameplayEffect)
	{
		if (!EffectClass) continue;

		const UGameplayEffect* EffectCDO = EffectClass.GetDefaultObject();
		FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
		ApplyGameplayEffectToSelf(EffectCDO, Level, ContextHandle);
		
	}
}

void UWarriorAbilitySystemComponent::GrantDefaultAbilitiesData(const UDataAsset_StartupDataBase* StartUpAbilitiesData,
                                                           int32 Level)
{
	GrantAbilities(StartUpAbilitiesData->GetActivateGivenAbilities(),1);
	GrantAbilities(StartUpAbilitiesData->GetReactiveGivenAbilities(),1);
	ApplyGameplayEffect(StartUpAbilitiesData->GetStartupGameplayEffects(), 1);
	
	//GrantStartupAbilitySets(Cast<UDataAsset_HeroStartupData>(StartUpAbilitiesData)->GetHeroStartupAbilitiesToGrant(), 1);
}

void UWarriorAbilitySystemComponent::GrantStartupAbilitySets(const TArray<FWarriorAbilitySet>& HeroStartupAbilitySet, const int32 Level)
{
	for	(const FWarriorAbilitySet&  AbilityData: HeroStartupAbilitySet)
	{
		if (AbilityData.GetAbilityToGrant() == nullptr)
		{
			continue;
		}
		GrantAbilityWithAbilityData(Level, AbilityData);
	}
}

void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid() == true)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Input Tag: %s"), *InputTag.ToString())
		for(const auto& AbilitySpec :	GetActivatableAbilities())
		{
			//UE_LOG(LogTemp, Warning, TEXT("AbilitySpec tag: %s"),*AbilitySpec.DynamicAbilityTags.ToString())
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				//UE_LOG(LogTemp, Warning, TEXT("OnAbilityInputPressed: %s"),*AbilitySpec.Ability.GetName())
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InputTag)
{
	
}

FGameplayAbilitySpecHandle UWarriorAbilitySystemComponent::GrantAbilityWithAbilityData(const int32 InLevel, const FWarriorAbilitySet& AbilitySet)
{
	FGameplayAbilitySpec Spec(AbilitySet.GetAbilityToGrant());
	Spec.SourceObject = GetAvatarActor();
	Spec.Level = InLevel;
	Spec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);	
	GiveAbility(Spec);
	return Spec.Handle;
}

TArray<FGameplayAbilitySpecHandle> UWarriorAbilitySystemComponent::GrandWeaponAbilities(const TArray<FWarriorAbilitySet>& InDefaultWeaponAbilities, const int32 InLevel)
{
	TArray<FGameplayAbilitySpecHandle> CachedAbilitySpec;
	for (const auto& AbilitySet : InDefaultWeaponAbilities)
	{
		if (AbilitySet.IsValid())
		{
			const FGameplayAbilitySpecHandle& SpecHandle = GrantAbilityWithAbilityData(InLevel, AbilitySet);
			CachedAbilitySpec.AddUnique(SpecHandle);
		}
	}
	return CachedAbilitySpec;
}

void UWarriorAbilitySystemComponent::RemoveGrantedHeroWeaponAbilities(UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
{
	for (const auto& AbilitySpec : InSpecHandlesToRemove)
	{
		if (AbilitySpec.IsValid())
		{
			ClearAbility(AbilitySpec);
		}
	}
	InSpecHandlesToRemove.Empty();
}

bool UWarriorAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
	check(AbilityTagToActivate.IsValid());

	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), FoundAbilitySpecs);

	if (FoundAbilitySpecs.IsEmpty() == false)
	{
		const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
		FGameplayAbilitySpec* SpecToActivate = FoundAbilitySpecs[RandomAbilityIndex];
		check(SpecToActivate);
		if (SpecToActivate->IsActive() == false)
		{
			return TryActivateAbility(SpecToActivate->Handle);
		}
	}
	return false;
}
