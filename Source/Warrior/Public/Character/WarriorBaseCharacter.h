// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "WarriorBaseCharacter.generated.h"
class UDataAsset_StartupDataBase;
class UWarriorAttributeSet;
class UWarriorAbilitySystemComponent;

UCLASS()
class WARRIOR_API AWarriorBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWarriorBaseCharacter();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
protected:

	virtual void PossessedBy(AController* NewController) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilitySystem, meta = (AllowPrivateAccess = "true"))
	UWarriorAbilitySystemComponent* WarriorAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AbilitySystem, meta = (AllowPrivateAccess = "true"))
	UWarriorAttributeSet* WarriorAttributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= CharacterData, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UDataAsset_StartupDataBase> StartUpAbilityData;
	
public:
	FORCEINLINE UWarriorAbilitySystemComponent* GetWarriorAbilitySystemComponent() const { return WarriorAbilitySystemComponent; }
	FORCEINLINE UWarriorAttributeSet* GetWarriorAttributesSet() const { return WarriorAttributes; }
};
