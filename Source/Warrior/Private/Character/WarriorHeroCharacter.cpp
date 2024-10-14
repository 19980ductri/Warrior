// Fill out your copyright notice in the Description page of Project Settings.


#include "Warrior/Public/Character/WarriorHeroCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/HeroCombatComponent.h"
#include "DataAssets/DataAsset_HeroStartupData.h"
#include "DataAssets/DataAsset_StartupDataBase.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/SpringArmComponent.h"

AWarriorHeroCharacter::AWarriorHeroCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42, 96);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("Camera Boom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.0f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("Follow Camera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 500.f, 0);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>("Combat Component");
	
}

void AWarriorHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AWarriorHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (StartUpAbilityData.IsNull() == false)
	{
		UDataAsset_StartupDataBase* LoadedData = StartUpAbilityData.LoadSynchronous();
		if (LoadedData)
		{
			//Grant ability
			GetWarriorAbilitySystemComponent()->GrantDefaultAbilitiesData(LoadedData, 1);
			GetWarriorAbilitySystemComponent()->GrantStartupAbilitySets(Cast<UDataAsset_HeroStartupData>(LoadedData)->GetHeroStartupAbilitiesToGrant(), 1);
		}
	}
}