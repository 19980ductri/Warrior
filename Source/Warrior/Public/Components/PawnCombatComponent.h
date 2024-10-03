// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PawnExtentionComponentBase.h"
#include "PawnCombatComponent.generated.h"


class AWarriorWeaponBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WARRIOR_API UPawnCombatComponent : public UPawnExtentionComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPawnCombatComponent();

public:
	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTag, AWarriorWeaponBase* InWeapon, bool bRegisterAsEquippedWeapon = false);

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorWeaponBase* GetCarriedWeaponByTag(FGameplayTag InWeaponTag);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Warrior|Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable, Category = "Warrior|Combat")
	AWarriorWeaponBase* GetCurrentEquippedWeapon();
	
private:

	TMap<FGameplayTag, AWarriorWeaponBase*> CharacterCarriedWeapons;
	
};
