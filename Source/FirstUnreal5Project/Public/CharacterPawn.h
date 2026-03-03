// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CharacterPawn.generated.h"

enum ProjectileType
{
	Light,
	Heavy
};

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
		MeleeFour
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

	/** Class reference to light projectile spawner to spawn light projectiles when firing light projectile action is triggered */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Weapons")
	TSubclassOf<AActor> LightProjectileClass;

	/** Class reference to heavy projectile spawner to spawn heavy projectiles when firing heavy projectile action is triggered */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Weapons")
	TSubclassOf<AActor> HeavyProjectileClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Attacks")
	TArray<FAttackInfo> AttackInfoArray;

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

	void FireProjectile(ProjectileType);

	void Attack(ESpawnableAttack::EType);

	UFUNCTION()
	void OnFeetOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	float JumpGravity;

	float JumpVelocity;

	bool bIsJumping;

	float JumpElapsedTime;

	float JumpStartZ;

	TMap<TEnumAsByte<ESpawnableAttack::EType>, TSubclassOf<AActor>> AttackMap;

	TMap<TEnumAsByte<ESpawnableAttack::EType>, FRotator> MeleeAttackRotatorMap;

	void EnableGravity(bool);

	void EnableFeetOverlapEvents(bool, bool);

	void SpawnAttack(ESpawnableAttack::EType, float, FRotator);
};
