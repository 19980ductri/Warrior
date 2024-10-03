// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Abilities/WarriorGameplayAbility.h"
#include "WarriorAbilitySystemComponent.generated.h"

class UDataAsset_HeroStartupData;
class UDataAsset_StartupDataBase;
/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UWarriorAbilitySystemComponent();

	void GrantDefaultAbilities(const UDataAsset_StartupDataBase* StartUpAbilitiesData, int32 level);

	void GrantStartupAbilitySets(const UDataAsset_HeroStartupData* HeroStartupAbilitySet, int32 level);

	void OnAbilityInputPressed(const FGameplayTag& InputTag);
	void OnAbilityInputReleased(const FGameplayTag& InputTag);
	
protected:
	void GrantAbilities(const TArray<TSubclassOf<UWarriorGameplayAbility>>& AbilitiesToGrant, int32 Level = 1);
	
	
};
