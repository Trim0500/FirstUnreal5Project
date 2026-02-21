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
		EnhancedInputComponenet->BindAction(MoveVertical, ETriggerEvent::Triggered, this, &APlayerCharacterController::EnhancedMoveVertically);
		EnhancedInputComponenet->BindAction(MoveHorizontal, ETriggerEvent::Triggered, this, &APlayerCharacterController::EnhancedMoveHorizontal);
		EnhancedInputComponenet->BindAction(Jump, ETriggerEvent::Triggered, this, &APlayerCharacterController::EnhancedJump);
		EnhancedInputComponenet->BindAction(FireLightProjectile, ETriggerEvent::Triggered, this, &APlayerCharacterController::EnhancedFireLightProjectile);
		EnhancedInputComponenet->BindAction(FireHeavyProjectile, ETriggerEvent::Triggered, this, &APlayerCharacterController::EnhancedFireHeavyProjectile);
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
		if (bIsJumpAvailable)
		{
			bIsJumpAvailable = false;

			UseJump();
		}
	}
}

void APlayerCharacterController::UseJump()
{
	ACharacterPawn* CharacterPawn = Cast<ACharacterPawn>(GetPawn());
	if (CharacterPawn != nullptr)
	{
		CharacterPawn->JumpPawn();
	}
}

void APlayerCharacterController::EnhancedFireLightProjectile(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("[APlayerCharacterController::EnhancedFireLightProjectile]: Starting fire light projectile..."));
	
	if (EInputActionValueType::Boolean == value.GetValueType())
	{
		UE_LOG(LogTemp, Warning, TEXT("[APlayerCharacterController::EnhancedFireLightProjectile]: Input accepted, will use logic..."));

		ACharacterPawn* CharacterPawn = Cast<ACharacterPawn>(GetPawn());
		if (CharacterPawn != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[APlayerCharacterController::EnhancedFireLightProjectile]: Found the character, using the respective action..."));

			CharacterPawn->FireLightProjectile();
		}
	}
}

void APlayerCharacterController::EnhancedFireHeavyProjectile(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("[APlayerCharacterController::EnhancedFireHeavyProjectile]: Starting fire heavy projectile..."));

	if (EInputActionValueType::Boolean == value.GetValueType())
	{
		UE_LOG(LogTemp, Warning, TEXT("[APlayerCharacterController::EnhancedFireHeavyProjectile]: Input accepted, will use logic..."));
	}
}
