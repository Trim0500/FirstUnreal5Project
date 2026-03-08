// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeAttack.h"

// Sets default values
AMeleeAttack::AMeleeAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMeleeAttack::BeginPlay()
{
	Super::BeginPlay();

	TotalFrameTime = 0.0f;

	bPastActiveFrames = false;

	bPastRecoveryFrames = false;

	bPastCancelRecoveryFrames = false;

	SetActorHiddenInGame(true);

	SetActorEnableCollision(false);

	PlayerCharacterControllerRef = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PlayerCharacterControllerRef != nullptr)
	{
		PlayerCharacterControllerRef->ApplyInputMappingContext(AttackInputMapping, 2, true);
	}
}

// Called every frame
void AMeleeAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO [PC-06]
	/*
	*	Implement the logic to track the player character actor location and ensure the melee attack actor is always in the correct position relative to the player character.
	* 
	*	NOTE: The exact distance in front of the player character ( in cm ) should be a constant value, declared and defined in a global header file
	*/

	TotalFrameTime += DeltaTime;

	if (TotalFrameTime >= LifetimeFrameThreshold && bNextAttackExists)
	{
		check(PlayerCharacterControllerRef);
		
		PlayerCharacterControllerRef->ApplyInputMappingContext(NextAttackInputMapping, 0, false);

		Destroy();
	}

	if (TotalFrameTime >= CancelRecoveryFrameThreshold && !bPastCancelRecoveryFrames)
	{
		bPastCancelRecoveryFrames = true;

		check(PlayerCharacterControllerRef);

		PlayerCharacterControllerRef->ApplyInputMappingContext(AttackInputMapping, 0, false);

		if (bNextAttackExists)
		{
			PlayerCharacterControllerRef->ApplyInputMappingContext(NextAttackInputMapping, 1, true);
		}
		else
		{
			Destroy();
		}
	}

	if (TotalFrameTime >= RecoveryFrameThreshold && !bPastRecoveryFrames)
	{
		bPastRecoveryFrames = true;

		SetActorHiddenInGame(true);

		SetActorEnableCollision(false);
	}

	if (TotalFrameTime >= StartUpFrameThreshold && !bPastActiveFrames)
	{
		bPastActiveFrames = true;

		SetActorHiddenInGame(false);

		SetActorEnableCollision(true);
	}
}
