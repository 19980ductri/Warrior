// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnUIComponent.h"
#include "HeroUIComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class WARRIOR_API UHeroUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHeroUIComponent();

	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentRangeChanged;

	
		
};
