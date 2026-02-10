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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Actions")
	UInputAction* IA_MoveHorizontal;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Actions")
	UInputAction* IA_MoveVertical;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Actions")
	UInputAction* IA_Jump;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Context")
	TSoftObjectPtr<UInputMappingContext> InputMapping;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Context")
	bool IsJumpAvailable;

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

	void EnhancedMoveHorizontal(const FInputActionValue& value);
	
	void MoveHorizontally(float);

	void EnhancedMoveVertically(const FInputActionValue& value);

	void MoveVertically(float);

	void EnhancedJump(const FInputActionValue& value);

	void Jump();
};
