// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/HeroAbilities/HeroGameplayAbility_TargetLock.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Character/WarriorHeroCharacter.h"
#include "Components/SizeBox.h"
#include "Controllers/WarriorHeroController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Widget/WarriorWidgetBase.h"

void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                      const FGameplayEventData* TriggerEventData)
{
	TryLockOnTarget();
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	CleanUpTargetLockAbility();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHeroGameplayAbility_TargetLock::TryLockOnTarget()
{
	GetAvailableActorsToLock();
	if (AvailableActorsToLock.IsEmpty())
	{
		CancelTargetLockAbility();
		return;
	}
	CurrentLockedActor = GetNearestFromAvailableActors(AvailableActorsToLock);
	if (CurrentLockedActor)
	{
		DrawTargetLockWidget();	
		SetTargetLockWidgetPosition();
	}
	else
	{
		CancelTargetLockAbility();
	}
	DrawDebugSphere(GetWorld(), CurrentLockedActor->GetActorLocation(), 100, 20, FColor::Yellow, false, 10.f);	
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsToLock()
{
	TArray<FHitResult> BoxTraceHits;
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetHeroCharacterFromActorInfo(),
		GetHeroCharacterFromActorInfo()->GetActorLocation(),
		GetHeroCharacterFromActorInfo()->GetActorLocation() + GetHeroCharacterFromActorInfo()->GetActorForwardVector() * BoxTraceDistance,
		TraceBoxSize/2.f,
		GetHeroCharacterFromActorInfo()->GetActorLocation().ToOrientationRotator(),
		BoxTraceChannel,
		false,
		TArray<AActor*>(),
		bShowPersistentDebugShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		BoxTraceHits,
		true
		);	
	for (const FHitResult& HitResult : BoxTraceHits)
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			if (HitActor == GetHeroCharacterFromActorInfo()) continue;
			AvailableActorsToLock.AddUnique(HitActor);
		}
	}
}

AActor* UHeroGameplayAbility_TargetLock::GetNearestFromAvailableActors(const TArray<AActor*>& InAvailableActors)
{
	float ClosestDistance = 0;
	return UGameplayStatics::FindNearestActor(GetHeroCharacterFromActorInfo()->GetActorLocation(), InAvailableActors, ClosestDistance);
}

void UHeroGameplayAbility_TargetLock::DrawTargetLockWidget()
{
	if (!DrawnTargetLockWidget)
	{
		check(TargetLockWidgetClass);
		DrawnTargetLockWidget = CreateWidget<UWarriorWidgetBase>(GetHeroControllerFromActorInfo(), TargetLockWidgetClass);
		check(DrawnTargetLockWidget);
		DrawnTargetLockWidget->AddToViewport();
	}
}

void UHeroGameplayAbility_TargetLock::SetTargetLockWidgetPosition()
{
	if (DrawnTargetLockWidget == nullptr || CurrentLockedActor == nullptr )
	{
		CancelTargetLockAbility();
		return;
	}
	FVector2d ScreenPos;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetHeroControllerFromActorInfo(),
		CurrentLockedActor->GetActorLocation(),
		ScreenPos,
		true);

	if (TargetLockWidgetSize == FVector2D::ZeroVector)
	{
		DrawnTargetLockWidget->WidgetTree->ForEachWidget(
		[this](UWidget* FoundWidget)
		{
			if (USizeBox* FoundSizeBox = Cast<USizeBox>(FoundWidget))
			{
				TargetLockWidgetSize.X = FoundSizeBox->GetWidthOverride();
				TargetLockWidgetSize.Y = FoundSizeBox->GetHeightOverride();
			}	
		});
	}
	//ScreenPos -= (TargetLockWidgetSize / 2.f);
	
	DrawnTargetLockWidget->SetPositionInViewport(ScreenPos, false);
	
}

void UHeroGameplayAbility_TargetLock::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UHeroGameplayAbility_TargetLock::CleanUpTargetLockAbility()
{
	AvailableActorsToLock.Empty();
	CurrentLockedActor = nullptr;
	if (DrawnTargetLockWidget)
	{
		DrawnTargetLockWidget->RemoveFromParent();
	}
}
