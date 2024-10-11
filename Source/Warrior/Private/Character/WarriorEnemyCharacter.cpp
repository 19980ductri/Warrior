
#include "Character/WarriorEnemyCharacter.h"

#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Components/EnemyCombatComponent.h"
#include "DataAssets/DataAsset_EnemyStartupData.h"
#include "DataAssets/DataAsset_StartupDataBase.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AWarriorEnemyCharacter::AWarriorEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 180.f, 0);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;
	
	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>("EnemyCombatComponent");	
}

void AWarriorEnemyCharacter::InitEnemyStartupData()
{
	if (StartUpAbilityData.IsNull())
	{
		return;
	}

	const FStreamableDelegate StreamableDelegate = FStreamableDelegate::CreateLambda([this] ()
		{
			if (UDataAsset_EnemyStartupData* LoadedData = CastChecked<UDataAsset_EnemyStartupData>(StartUpAbilityData.Get()))
			{
				WarriorAbilitySystemComponent->GrantAbilities(LoadedData->GetActivateGivenAbilities(),1);
				WarriorAbilitySystemComponent->GrantAbilities(LoadedData->GetReactiveGivenAbilities(),1);
				
			}
		});
	
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		StartUpAbilityData.ToSoftObjectPath(), StreamableDelegate);
	
}


