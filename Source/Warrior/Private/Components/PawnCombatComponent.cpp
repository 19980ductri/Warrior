// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PawnCombatComponent.h"

#include "Item/WarriorWeaponBase.h"


// Sets default values for this component's properties
UPawnCombatComponent::UPawnCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTag, AWarriorWeaponBase* InWeapon,
	bool bRegisterAsEquippedWeapon)
{
	if (CharacterCarriedWeapons.Contains(InWeaponTag))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has been registered"), *InWeaponTag.ToString())
	}
	if (InWeapon)
	{
		CharacterCarriedWeapons.Emplace(InWeaponTag, InWeapon);
		if (bRegisterAsEquippedWeapon)
		{
			CurrentEquippedWeaponTag = InWeaponTag;
		}
	}
}

AWarriorWeaponBase* UPawnCombatComponent::GetCarriedWeaponByTag(FGameplayTag InWeaponTag)
{
	if (CharacterCarriedWeapons.Contains(InWeaponTag))
	{
		AWarriorWeaponBase* CarriedWeapon = CharacterCarriedWeapons[InWeaponTag];
		return CarriedWeapon;
	}
	return  nullptr;
}

AWarriorWeaponBase* UPawnCombatComponent::GetCurrentEquippedWeapon()
{
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		return  nullptr;
	}
	return GetCarriedWeaponByTag(CurrentEquippedWeaponTag);
}
