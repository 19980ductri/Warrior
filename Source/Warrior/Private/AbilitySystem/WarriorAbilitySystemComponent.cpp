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
		FGameplayAbilitySpec AbilitySpec(AbilityData.GetAbilityToGrant());
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = level;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityData.InputTag);
		GiveAbility(AbilitySpec);
	}
	
}

void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid() == true)
	{
		for(const auto& AbilitySpec :	GetActivatableAbilities())
		{
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InputTag)
{
	
}
