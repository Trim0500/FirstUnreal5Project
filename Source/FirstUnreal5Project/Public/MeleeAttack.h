// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeleeAttack.generated.h"

UCLASS()
class FIRSTUNREAL5PROJECT_API AMeleeAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeleeAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Input Mapping Context asset for attack player character state */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Context")
	TSoftObjectPtr<UInputMappingContext> AttackInputMapping;

	/** Definition for how long to wait until active attack after completed input  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack Frame Data")
	float StartUpFrameThreshold;

	/** Definition for how long to hold active attack after startup  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack Frame Data")
	float ActiveFrameThreshold;

	/** Definition for how long until enter recovery frames */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack Frame Data")
	float RecoveryFrameThreshold;
	
	/** Definition for how long to wait after starting recovery until character can move again  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack Frame Data")
	float CancelRecoveryFrameThreshold;
	
	/** Definition for frame limit in recovery to use next attack in ( possible ) sequence  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack Frame Data")
	float LifetimeFrameThreshold;
	
	/** Definition for frame limit in recovery to use next attack in ( possible ) sequence  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack Properties")
	bool bNextAttackExists;
	
	/** Input Mapping Context asset for next attack player character state */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Context")
	TSoftObjectPtr<UInputMappingContext> NextAttackInputMapping;

	/** Definition for damage to apply to character when hit by attack  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attack Properties")
	int Power;
private:
	float TotalFrameTime;

	bool bPastActiveFrames;
	
	bool bPastRecoveryFrames;
	
	bool bPastCancelRecoveryFrames;
};
