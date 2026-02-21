// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
	
	/** Input Action asset to map controller input to firing light projectile action */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Actions ( Attacks )")
	UInputAction* FireLightProjectile;

	/** Input Action asset to map controller input to firing heavy projectile action */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Actions ( Attacks )")
	UInputAction* FireHeavyProjectile;

	/** Input Mapping Context asset for neutral player character state */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Context")
	TSoftObjectPtr<UInputMappingContext> InputMapping;
	
	/** Can this controller use a jump action? */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Flags")
	bool bIsJumpAvailable;

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
};
