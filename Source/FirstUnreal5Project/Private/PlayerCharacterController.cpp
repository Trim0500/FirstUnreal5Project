// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"

#include "PlayerCharacterController.h"
#include "CharacterPawn.h"

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent != nullptr);

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMapping.IsNull())
			{
				InputSystem->AddMappingContext(InputMapping.LoadSynchronous(), 1);
			}
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponenet = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponenet->BindAction(IA_MoveVertical, ETriggerEvent::Triggered, this, &APlayerCharacterController::EnhancedMoveVertically);
		EnhancedInputComponenet->BindAction(IA_MoveHorizontal, ETriggerEvent::Triggered, this, &APlayerCharacterController::EnhancedMoveHorizontal);
		EnhancedInputComponenet->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &APlayerCharacterController::EnhancedJump);
	}
}

void APlayerCharacterController::EnhancedMoveHorizontal(const FInputActionValue& value)
{
	if (value.GetValueType() == EInputActionValueType::Axis1D)
	{
		float inputVector = value.Get<float>();
		MoveHorizontally(inputVector);
	}
}

void APlayerCharacterController::MoveHorizontally(float _inputVector)
{
	if (_inputVector != 0)
	{
		ACharacterPawn* CharacterPawn = Cast<ACharacterPawn>(GetPawn());
		if (CharacterPawn != nullptr)
		{
			CharacterPawn->MovePawnHorizontally(_inputVector);
		}
	}
}

void APlayerCharacterController::EnhancedMoveVertically(const FInputActionValue& value)
{
	if (value.GetValueType() == EInputActionValueType::Axis1D)
	{
		float inputVector = value.Get<float>();
		MoveVertically(inputVector);
	}
}

void APlayerCharacterController::MoveVertically(float _inputVector)
{
	if (_inputVector != 0)
	{
		ACharacterPawn* CharacterPawn = Cast<ACharacterPawn>(GetPawn());
		if (CharacterPawn != nullptr)
		{
			CharacterPawn->MovePawnVertically(_inputVector);
		}
	}
}

void APlayerCharacterController::EnhancedJump(const FInputActionValue& value)
{
	if (value.GetValueType() == EInputActionValueType::Boolean)
	{
		/*
		* [PC-02]: TODO: Add a check for IsJumpAvailable before calling Jump() and if available, call Jump() and set IsJumpAvailable to false.
		*/
		if (IsJumpAvailable)
		{
			IsJumpAvailable = false;

			Jump();
		}
	}
}

void APlayerCharacterController::Jump()
{
	ACharacterPawn* CharacterPawn = Cast<ACharacterPawn>(GetPawn());
	if (CharacterPawn != nullptr)
	{
		CharacterPawn->JumpPawn();
	}
}
