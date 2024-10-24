// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "InputActionValue.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "GameFramework/PlayerController.h"
#include "WarriorHeroController.generated.h"

class UWarriorAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class WARRIOR_API AWarriorHeroController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:

	virtual void SetupInputComponent() override;
	void Input_AbilityInputPressed(FGameplayTag InputTag);
	void Input_AbilityInputReleased(FGameplayTag InputTag);
	
	UWarriorAbilitySystemComponent* GetWarriorAbilitySystemComponent();

	virtual FGenericTeamId GetGenericTeamId() const override;
protected:
	
	UFUNCTION()
	void MoveCharacter(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);

	
private:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CharacterData, meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	UPROPERTY(BlueprintReadOnly, Category = CharacterData, meta = (AllowPrivateAccess = "true"))
	UWarriorAbilitySystemComponent* WarriorAbilitySystemComponent;
	
	FGenericTeamId HeroTeamID;
	
};
