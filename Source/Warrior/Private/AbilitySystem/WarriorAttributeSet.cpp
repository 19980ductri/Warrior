// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WarriorAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "WarriorFunctionLibrary.h"
#include "WarriorGameplayTags.h"
#include "Components/UIComponents/HeroUIComponent.h"
#include "Components/UIComponents/PawnUIComponent.h"
#include "Interfaces/PawnUIInterface.h"

UWarriorAttributeSet::UWarriorAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

void UWarriorAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	/*
	if (!CachedPawnUIInterface.IsValid())
	{
		//CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
		CachedPawnUIInterface = Cast<IPawnUIInterface>(	Data.Target.GetAvatarActor());
	}
	check(CachedPawnUIInterface.IsValid());
	const UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();
	*/

	if (!CachedPawnUIInterface.IsValid())
	{
		// Use TWeakInterfacePtr if Data.Target.GetAvatarActor() implements IPawnUIInterface
		CachedPawnUIInterface = Cast<IPawnUIInterface>(Data.Target.GetAvatarActor());
		if (!CachedPawnUIInterface.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("CachedPawnUIInterface is invalid! Avatar actor might not implement IPawnUIInterface."));
			return;
		}
	}

	// Proceed only if CachedPawnUIInterface is valid
	const UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();
	if (!PawnUIComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("PawnUIComponent is null! Make sure the avatar has a valid UI component."));
		return;
	}


	
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth()/GetMaxHealth());
	}

	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());
		SetCurrentRage(NewCurrentRage);
		const UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent();
		HeroUIComponent->OnCurrentRangeChanged.Broadcast(GetCurrentRage()/GetMaxRage());
	}

	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();
		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());
		
		SetCurrentHealth(NewCurrentHealth);
		PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth()/GetMaxHealth());
		
		if (NewCurrentHealth == 0.f)
		{
			//See Triggers->Triger Source in GA
			UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), WarriorGameplayTags::Shared_Status_Dead);
		}
	}
}

