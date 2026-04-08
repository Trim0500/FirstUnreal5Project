// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TimerComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FirstUnreal5GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREAL5PROJECT_API AFirstUnreal5GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer Settings")
	float TimerDuration;

	UFUNCTION()
	void OnTimerFinished();

private:
	UTimerComponent* TimerComponent;
};
