// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CharacterPawn.generated.h"

UENUM(BlueprintType)
namespace ESpawnableAttack
{
	enum EType
	{
		ProjectileLight,
		ProjectileHeavy,
		MeleeOne,
		MeleeTwo,
		MeleeThree,
		MeleeFour,
		Lunge
	};
}

USTRUCT(BlueprintType)
struct FAttackInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Attacks")
	TEnumAsByte<ESpawnableAttack::EType> AttackType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Attacks")
	TSubclassOf<AActor> AttackClass;
};

struct FDodgeInfo
{
	FVector StartLocation;
	float DodgeDistanceScale;
	FVector TargetDirection;
	float ElapsedTime;
	float MaxDodgeTime;
	bool bUseEaseOut;
};

struct LockOnTargetInfo
{
	AActor* LockOnCandidate;

	FVector TargetTransform;
};

UCLASS()
class FIRSTUNREAL5PROJECT_API ACharacterPawn : public APawn
{
	GENERATED_BODY()

public:
	/** Set scale for horizontal movement ( in cm ) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Movement")
	float MoveScale;
	
	/** Set scale for lock-on movement reduction ( [0.0 - 1.0] ) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Movement")
	float LockOnMoveReductionScale;

	/** Set peak height to add when jumping ( in cm ) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Movement")
	float PeakJumpHeight;

	/** Set time to reach peak height when jumping ( in sec ) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Movement")
	float TimeToPeakJump;
	
	/** Set scale value to apply to dodge action ( in cm ) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Movement")
	float DodgeDistanceScale;
	
	/** Set time value for max dodge time ( in sec ) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Movement")
	float MaxDodgeTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Attacks")
	TArray<FAttackInfo> AttackInfoArray;

	/** Set scale value to apply to lunge action ( in cm ) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Movement")
	float LungeDistanceScale;

	/** Set time value for max lunge time ( in sec ) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Movement")
	float MaxLungeTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Properties")
	float MaxLockOnDistance;

	/** Input Mapping Context asset for dodge player character state */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Context")
	TSoftObjectPtr<UInputMappingContext> DodgeInputMapping;

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

	void Attack(ESpawnableAttack::EType);

	void ToggleLockOn(bool);

	void CycleLockOnTarget();

	void Dodge(bool);

	void Lunge();

	void CancelLunge();

	UFUNCTION()
	void OnFeetOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	float JumpGravity;

	float JumpVelocity;

	bool bIsJumping;

	float JumpElapsedTime;

	float JumpStartZ;

	FDodgeInfo CurrentDodgeInfo;

	bool bIsDodging;

	TMap<TEnumAsByte<ESpawnableAttack::EType>, TSubclassOf<AActor>> AttackMap;

	TMap<TEnumAsByte<ESpawnableAttack::EType>, FRotator> MeleeAttackRotatorMap;

	TArray<LockOnTargetInfo> LockOnTargets;

	int CurrentLockOnTargetIndex;

	bool bIsLockOnActive;

	void SetPawnMeshRotator(float, float);

	void EnableGravity(bool);

	void EnableFeetOverlapEvents(bool, bool);

	float EaseOut(float);

	FVector GetDodgeLocation(FDodgeInfo);

	void ApplyDodge(FDodgeInfo);

	void SpawnAttack(ESpawnableAttack::EType, float, FRotator);

	void CalculateLockOnTargets(bool);

	void AdjustCameraForLockOn(FVector);
};
