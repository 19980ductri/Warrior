// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "HeroGameplayAbility_TargetLock.generated.h"

class UWarriorWidgetBase;
/**
 * 
 */
UCLASS()
class WARRIOR_API UHeroGameplayAbility_TargetLock : public UWarriorHeroGameplayAbility
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


	void CancelTargetLockAbility();
	void CleanUpTargetLockAbility();	
	
private:

	void TryLockOnTarget();
	void GetAvailableActorsToLock();
	AActor* GetNearestFromAvailableActors(const TArray<AActor*>& InAvailableActors);
	void DrawTargetLockWidget();
	void SetTargetLockWidgetPosition();
	
	UPROPERTY(EditDefaultsOnly, Category= "TargetLock")
	float BoxTraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "TargetLock")
	FVector TraceBoxSize = FVector(5000.f,5000.f,300.f);

	UPROPERTY(EditDefaultsOnly, Category= "TargetLock")
	TArray<TEnumAsByte<EObjectTypeQuery>> BoxTraceChannel;

	UPROPERTY(EditDefaultsOnly, Category= "TargetLock")
	bool bShowPersistentDebugShape=true;

	UPROPERTY(EditDefaultsOnly, Category= "TargetLock")
	TSubclassOf<UWarriorWidgetBase> TargetLockWidgetClass;
	 
	UPROPERTY()
	TArray<AActor*> AvailableActorsToLock;
	
	UPROPERTY()
	AActor* CurrentLockedActor;

	UPROPERTY()
	UWarriorWidgetBase* DrawnTargetLockWidget;

	UPROPERTY()
	FVector2D TargetLockWidgetSize = FVector2D::ZeroVector;
	
};
