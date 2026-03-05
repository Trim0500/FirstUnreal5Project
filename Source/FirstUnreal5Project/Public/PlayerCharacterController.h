// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CharacterPawn.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "PlayerCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREAL5PROJECT_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	/** Input Action asset to map controller input to horizontal move action */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Actions ( Movement )")
	UInputAction* MoveHorizontal;

	/** Input Action asset to map controller input to vertical move action */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Actions ( Movement )")
	UInputAction* MoveVertical;
	
	/** Input Action asset to map controller input to jump action */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Actions ( Movement )")
	UInputAction* Jump;
	
	/** Input Action asset to map controller input to dodge action */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Actions ( Movement )")
	UInputAction* Dodge;
	
	/** Input Action asset to map controller input to firing light projectile action */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Actions ( Attacks )")
	UInputAction* FireLightProjectile;

	/** Input Action asset to map controller input to firing heavy projectile action */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Actions ( Attacks )")
	UInputAction* FireHeavyProjectile;
	
	/** Input Action asset to map controller input to using melee attack ( 1/4 ) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Actions ( Attacks )")
	UInputAction* MeleeAttackOne;
	
	/** Input Action asset to map controller input to using melee attack ( 2/4 ) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Actions ( Attacks )")
	UInputAction* MeleeAttackTwo;
	
	/** Input Action asset to map controller input to using melee attack ( 3/4 ) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Actions ( Attacks )")
	UInputAction* MeleeAttackThree;
	
	/** Input Action asset to map controller input to using melee attack ( 4/4 ) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Actions ( Attacks )")
	UInputAction* MeleeAttackFour;
	
	/** Input Action asset to map controller input to using lunge attack */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Actions ( Attacks )")
	UInputAction* Lunge;
	
	/** Input Action asset to map controller input to entering lock-on state */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Actions ( State )")
	UInputAction* BeginLockOn;
	
	/** Input Action asset to map controller input to leaving lock-on state */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Actions ( State )")
	UInputAction* EndLockOn;
	
	/** Input Action asset to map controller input to changing lock-on target */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Actions ( State )")
	UInputAction* ChangeLockOnTarget;

	/** Input Mapping Context asset for neutral player character state */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Context")
	TSoftObjectPtr<UInputMappingContext> InputMapping;
	
	/** Input Mapping Context asset for lock-on player character state */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Context")
	TSoftObjectPtr<UInputMappingContext> LockOnInputMapping;
	
	/** Can this controller use a jump action? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Flags")
	bool bIsJumpAvailable;

	/** Can this controller use a melee action? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Flags")
	bool bIsMeleeAvailable;
	
	/** Is this controller in lock-on state? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Flags")
	bool bIsInLockOn;

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

	void EnhancedMoveHorizontal(const FInputActionValue& value);
	
	void MoveHorizontally(float);

	void EnhancedMoveVertically(const FInputActionValue& value);

	void MoveVertically(float);

	void EnhancedJump(const FInputActionValue& value);

	void UseJump();

	void EnhancedFireLightProjectile(const FInputActionValue& value);

	void EnhancedFireHeavyProjectile(const FInputActionValue& value);
	
	void EnhancedUseMeleeAttackOne(const FInputActionValue& value);

	void EnhancedUseMeleeAttackTwo(const FInputActionValue& value);

	void EnhancedUseMeleeAttackThree(const FInputActionValue& value);

	void EnhancedUseMeleeAttackFour(const FInputActionValue& value);

	void EnhancedUseDodge(const FInputActionValue& value);

	void EnhancedUseLunge(const FInputActionValue& value);
	
	void EnhancedBeginLockOn(const FInputActionValue& value);
	
	void EnhancedEndLockOn(const FInputActionValue& value);
	
	void EnhancedChangeLockOnTarget(const FInputActionValue& value);

	void ApplyInputMappingContext(TSoftObjectPtr<UInputMappingContext>&, int, bool);
private:
	void UseAttack(const FInputActionValue&, ESpawnableAttack::EType);
};
