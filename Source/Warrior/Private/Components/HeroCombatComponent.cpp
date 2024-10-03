// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HeroCombatComponent.h"

#include "Item/WarriorHeroWeapon.h"


AWarriorHeroWeapon* UHeroCombatComponent::GetHeroCarriedWeaponByTag(FGameplayTag InWeaponTag)
{
	return Cast<AWarriorHeroWeapon>(GetCarriedWeaponByTag(InWeaponTag));
}


