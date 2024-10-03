﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WarriorStructTypes.h"
#include "WarriorWeaponBase.h"
#include "WarriorHeroWeapon.generated.h"

class UBoxComponent;

UCLASS()
class WARRIOR_API AWarriorHeroWeapon : public AWarriorWeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWarriorHeroWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FWarriorHeroWeaponData WeaponData;
	
protected:


};
