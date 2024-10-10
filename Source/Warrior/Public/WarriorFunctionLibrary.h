// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WarriorFunctionLibrary.generated.h"

UENUM()
enum class EWarriorConfirmType : uint8
{
	Yes,
	No
};


class UWarriorAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static UWarriorAbilitySystemComponent* NativeGetWarriorASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLib")
	static void AddGameplayTagToActorIfNone(AActor* InActor,FGameplayTag InTag);
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLib")
	static void RemoveGameplayTagIfFound(AActor* InActor, FGameplayTag InTag);

	static bool NativeDoesActorHasTag(AActor* InActor, FGameplayTag InTag);
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLib", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag InTag, EWarriorConfirmType& OutConfirmType);
};
