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

void UWarriorAbilitySystemComponent::GrantDefaultAbilities(const UDataAsset_StartupDataBase* StartUpAbilitiesData,
	int32 level)
{
	GrantAbilities(StartUpAbilitiesData->GetActivateGivenAbilities(),1);
	GrantAbilities(StartUpAbilitiesData->GetReactiveGivenAbilities(),1);
	GrantStartupAbilitySets(Cast<UDataAsset_HeroStartupData>(StartUpAbilitiesData), 1);
}

void UWarriorAbilitySystemComponent::GrantStartupAbilitySets(const UDataAsset_HeroStartupData* HeroStartupAbilitySet,
	int32 level)
{
	if (HeroStartupAbilitySet == nullptr)
	{
		return;
	}
	for	(const auto&  AbilityData: HeroStartupAbilitySet->GetHeroStartupAbilitiesToGrant())
	{
		if (AbilityData.GetAbilityToGrant() == nullptr)
		{
			continue;
		}
		GrantAbilityWithAbilityData(level, AbilityData);
	}
	
}

void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid() == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Tag: %s"), *InputTag.ToString())
		for(const auto& AbilitySpec :	GetActivatableAbilities())
		{
			//UE_LOG(LogTemp, Warning, TEXT("AbilitySpec tag: %s"),*AbilitySpec.DynamicAbilityTags.ToString())
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				UE_LOG(LogTemp, Warning, TEXT("OnAbilityInputPressed: %s"),*AbilitySpec.Ability.GetName())
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InputTag)
{
	
}

FGameplayAbilitySpecHandle UWarriorAbilitySystemComponent::GrantAbilityWithAbilityData(
	int32 InLevel, const FWarriorAbilitySet& AbilitySet)
{
	FGameplayAbilitySpec Spec(AbilitySet.GetAbilityToGrant());
	Spec.SourceObject = GetAvatarActor();
	Spec.Level = InLevel;
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Spec.DynamicAbilityTags.ToString());
	Spec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);	
	GiveAbility(Spec);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Spec.DynamicAbilityTags.ToString());

	return Spec.Handle;
}

TArray<FGameplayAbilitySpecHandle> UWarriorAbilitySystemComponent::GrandWeaponAbilities(
	const TArray<FWarriorAbilitySet>& InDefaultWeaponAbilities,
	const int32 InLevel)
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

void UWarriorAbilitySystemComponent::RemoveGrantedHeroWeaponAbilities(
	UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
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
