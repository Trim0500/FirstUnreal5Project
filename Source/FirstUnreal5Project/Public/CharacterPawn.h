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
	/** Set scale for horizontal movement ( in cm ) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Movement")
	float MoveScale;

	/** Set peak height to add when jumping ( in cm ) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Movement")
	float PeakJumpHeight;

	/** Set time to reach peak height when jumping ( in sec ) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Movement")
	float TimeToPeakJump;

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

	UFUNCTION()
	void OnFeetOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	float JumpGravity;

	float JumpVelocity;

	bool bIsJumping;

	float JumpElapsedTime;

	float JumpStartZ;

	void EnableGravity(bool);

	void EnableFeetOverlapEvents(bool, bool);

};
