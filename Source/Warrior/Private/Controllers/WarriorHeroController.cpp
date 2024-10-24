// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/WarriorHeroController.h"

#include "EnhancedInputSubsystems.h"
#include "WarriorGameplayTags.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Character/WarriorBaseCharacter.h"
#include "Components/Input/WarriorInputComponent.h"
#include "GameFramework/Character.h"


void AWarriorHeroController::SetupInputComponent()
{
	Super::SetupInputComponent();
	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* InputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(InputLocalPlayerSubsystem);

	InputLocalPlayerSubsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, -1);
	UWarriorInputComponent* WarriorInputComponent =	CastChecked<UWarriorInputComponent>(InputComponent);

	WarriorInputComponent->BindNativeInputAction(InputConfigDataAsset, WarriorGameplayTags::InputTag_Move,
		ETriggerEvent::Triggered, this, &ThisClass::MoveCharacter);
	WarriorInputComponent->BindNativeInputAction(InputConfigDataAsset, WarriorGameplayTags::InputTag_Look,
		ETriggerEvent::Triggered, this, &ThisClass::Look);

	WarriorInputComponent->BindAbilityInputAction(InputConfigDataAsset, this,
		&ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);


	HeroTeamID = FGenericTeamId(0);
}

void AWarriorHeroController::MoveCharacter(const FInputActionValue& InputActionValue)
{
	// Example: Use Value.Get<float>() to retrieve the float value
	const FVector2d MovementVector = InputActionValue.Get<FVector2d>();
	const FRotator MovementRotation(0, GetControlRotation().Yaw, 0);

	if (MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		GetCharacter()->AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	if (MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		GetCharacter()->AddMovementInput(RightDirection, MovementVector.X);
	}

}

void AWarriorHeroController::Look(const FInputActionValue& InputActionValue)
{
	const FVector2d& LookDirection = InputActionValue.Get<FVector2d>();
	
	if (LookDirection.X != 0.f)
	{
		AddYawInput(LookDirection.X);
	}
	if (LookDirection.Y != 0.f)
	{
		AddPitchInput(LookDirection.Y);
	}
}

void AWarriorHeroController::Input_AbilityInputPressed(FGameplayTag InputTag)
{	
	//Cast<AWarriorBaseCharacter>(GetCharacter())->GetWarriorAbilitySystemComponent()->OnAbilityInputPressed(InputTag);
	GetWarriorAbilitySystemComponent()->OnAbilityInputPressed(InputTag);
	
	
}

void AWarriorHeroController::Input_AbilityInputReleased(FGameplayTag InputTag)
{
	///Cast<AWarriorBaseCharacter>(GetCharacter())->GetWarriorAbilitySystemComponent()->OnAbilityInputReleased(InputTag);
	GetWarriorAbilitySystemComponent()->OnAbilityInputReleased(InputTag);
}

UWarriorAbilitySystemComponent* AWarriorHeroController::GetWarriorAbilitySystemComponent()
{
	if (WarriorAbilitySystemComponent == nullptr)
	{
		WarriorAbilitySystemComponent =	Cast<AWarriorBaseCharacter>(GetCharacter())->GetWarriorAbilitySystemComponent();
		return WarriorAbilitySystemComponent;
	}
	return WarriorAbilitySystemComponent;
}

FGenericTeamId AWarriorHeroController::GetGenericTeamId() const
{
	return HeroTeamID;
}
