// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WarriorEnumTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WarriorFunctionLibrary.generated.h"

class UPawnCombatComponent;



class UWarriorAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static UWarriorAbilitySystemComponent* NativeGetWarriorAscFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLib")
	static void AddGameplayTagToActorIfNone(AActor* InActor,FGameplayTag InTag);

	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLib")
	static void RemoveGameplayTagIfFound(AActor* InActor, FGameplayTag InTag);

	static bool NativeDoesActorHasTag(AActor* InActor, FGameplayTag InTag);
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLib", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag InTag, EWarriorConfirmType& OutConfirmType);
	
	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLib", meta = (DisplayName = "Get Pawn Combat Component From Actor Info", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActorInfo(AActor* InActor, EWarriorValidType& OutValidType);
	
};
