// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CharacterPawn.generated.h"

UCLASS()
class FIRSTUNREAL5PROJECT_API ACharacterPawn : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Movement")
	float MoveScale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Movement")
	float PeakJumpHeight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Movement")
	float TimeToPeakJump;

	/*UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Player Collission")
	UBoxComponent* Feet;*/

	// Sets default values for this pawn's properties
	ACharacterPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MovePawnHorizontally(float);

	void MovePawnVertically(float);

	void JumpPawn();

	void ApplyJumpToZ(float, float, float);

	/*
	* NOTE: Unsure how to set up the collision component to detect collision with the ground. Will need to research this further.
	*/
	/*UFUNCTION()
	void OnFeetOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/

private:
	float JumpGravity;

	float JumpVelocity;

	bool bIsJumping;

	float JumpElapsedTime;

	float JumpStartZ;

	void EnableGravity(bool);

};
