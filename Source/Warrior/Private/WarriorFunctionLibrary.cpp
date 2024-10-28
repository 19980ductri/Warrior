// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"

UWarriorAbilitySystemComponent* UWarriorFunctionLibrary::NativeGetWarriorAscFromActor(AActor* InActor)
{
	check(InActor);
	
	return CastChecked<UWarriorAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
	
}

void UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, const FGameplayTag InTag)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorAscFromActor(InActor);

	if (ASC->HasMatchingGameplayTag(InTag) == false)
	{
		ASC->AddLooseGameplayTag(InTag);
	}
}

void UWarriorFunctionLibrary::RemoveGameplayTagIfFound(AActor* InActor, const FGameplayTag InTag)
{
	UWarriorAbilitySystemComponent* ASC = NativeGetWarriorAscFromActor(InActor);

	if (ASC->HasMatchingGameplayTag(InTag) == true)
	{
		ASC->RemoveLooseGameplayTag(InTag);
	}
}

bool UWarriorFunctionLibrary::NativeDoesActorHasTag(AActor* InActor, FGameplayTag InTag)
{
	const UWarriorAbilitySystemComponent* ASC = NativeGetWarriorAscFromActor(InActor);
	return ASC->HasMatchingGameplayTag(InTag);	
}

void UWarriorFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, const FGameplayTag InTag,
	EWarriorConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHasTag(InActor, InTag) ? EWarriorConfirmType::Yes : EWarriorConfirmType::No;
			
}

UPawnCombatComponent* UWarriorFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	check(InActor);
	if (const IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetCombatComponent();
	}
	return nullptr;
}

UPawnCombatComponent* UWarriorFunctionLibrary::BP_GetPawnCombatComponentFromActorInfo(AActor* InActor,
	EWarriorValidType& OutValidType)
{
	UPawnCombatComponent* PawnCombatComponent = NativeGetPawnCombatComponentFromActor(InActor);
	OutValidType = PawnCombatComponent ? EWarriorValidType::Valid : EWarriorValidType::Invalid;
	return PawnCombatComponent;
}

bool UWarriorFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
	check(QueryPawn);
	check(TargetPawn);
	
	IGenericTeamAgentInterface* QueryTeamAgentInterface = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	if (QueryTeamAgentInterface == nullptr)
		return false;

	IGenericTeamAgentInterface* TargetTeamAgentInterface = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());
	if (TargetTeamAgentInterface == nullptr)
		return false;

	return QueryTeamAgentInterface->GetGenericTeamId() != TargetTeamAgentInterface->GetGenericTeamId();
	
}
