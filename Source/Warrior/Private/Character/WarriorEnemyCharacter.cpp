
#include "Character/WarriorEnemyCharacter.h"

#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Components/EnemyCombatComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/UIComponents/EnemyUIComponent.h"
#include "DataAssets/DataAsset_EnemyStartupData.h"
#include "DataAssets/DataAsset_StartupDataBase.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/WarriorWidgetBase.h"

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
	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>("UI Component");
	EnemyHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Health Widget Component");
	EnemyHealthWidgetComponent->SetupAttachment(GetMesh());
} 

UPawnCombatComponent* AWarriorEnemyCharacter::GetCombatComponent() const
{
	return EnemyCombatComponent;
}

UPawnUIComponent* AWarriorEnemyCharacter::GetPawnUIComponent() const
{
	return EnemyUIComponent;
}

UEnemyUIComponent* AWarriorEnemyCharacter::GetEnemyUIComponent() const
{
	return EnemyUIComponent;
}


void AWarriorEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	UWarriorWidgetBase* HealthWidget = Cast<UWarriorWidgetBase>(EnemyHealthWidgetComponent->GetUserWidgetObject());
	if (HealthWidget)
	{
		HealthWidget->InitEnemyCreatedWidget(this);
	}
}

void AWarriorEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitEnemyStartupData();
}

void AWarriorEnemyCharacter::InitEnemyStartupData()
{
	if (StartUpAbilityData.IsNull())
	{
		return;
	}

	const FStreamableDelegate StreamableDelegate = FStreamableDelegate::CreateLambda([this] ()
		{
			if (const UDataAsset_EnemyStartupData* LoadedData = CastChecked<UDataAsset_EnemyStartupData>(StartUpAbilityData.Get()))
			{
				WarriorAbilitySystemComponent->GrantDefaultAbilitiesData(LoadedData,1);
				WarriorAbilitySystemComponent->GrantAbilities(LoadedData->GetEnemyCombatAbilities(),1);
				//WarriorAbilitySystemComponent->GrantAbilities(LoadedData->GetEnemyCombatAbilities(),1);
			}
		});
	
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		StartUpAbilityData.ToSoftObjectPath(), StreamableDelegate);
	
}


