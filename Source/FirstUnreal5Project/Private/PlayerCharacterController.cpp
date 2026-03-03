// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"

#include "PlayerCharacterController.h"

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
		EnhancedInputComponenet->BindAction(MeleeAttackOne, ETriggerEvent::Triggered, this, &APlayerCharacterController::EnhancedUseMeleeAttackOne);
		EnhancedInputComponenet->BindAction(MeleeAttackTwo, ETriggerEvent::Triggered, this, &APlayerCharacterController::EnhancedUseMeleeAttackTwo);

		/*
		* [PC-05]: TODO
		*			Bind melee attack input actions to their respective functions here once they are implemented
		*/
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

			bIsMeleeAvailable = false;

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
	if (EInputActionValueType::Boolean == value.GetValueType())
	{
		ACharacterPawn* CharacterPawn = Cast<ACharacterPawn>(GetPawn());
		if (CharacterPawn != nullptr)
		{
			CharacterPawn->FireProjectile(ProjectileType::Light);
		}
	}
}

void APlayerCharacterController::EnhancedFireHeavyProjectile(const FInputActionValue& value)
{
	if (EInputActionValueType::Boolean == value.GetValueType())
	{
		ACharacterPawn* CharacterPawn = Cast<ACharacterPawn>(GetPawn());
		if (CharacterPawn != nullptr)
		{
			CharacterPawn->FireProjectile(ProjectileType::Heavy);
		}
	}
}

void APlayerCharacterController::EnhancedUseMeleeAttackOne(const FInputActionValue& value)
{
	UseMeleeAttack(value, ESpawnableAttack::MeleeOne);
}

void APlayerCharacterController::EnhancedUseMeleeAttackTwo(const FInputActionValue& value)
{
	UseMeleeAttack(value, ESpawnableAttack::MeleeTwo);
}

void APlayerCharacterController::EnhancedUseMeleeAttackThree(const FInputActionValue& value)
{
	/*
	* [PC-05]: TODO
	*			Call the character pawn's melee attack function for the third melee attack once it is implemented
	*/
}

void APlayerCharacterController::EnhancedUseMeleeAttackFour(const FInputActionValue& value)
{
	/*
	* [PC-05]: TODO
	*			Call the character pawn's melee attack function for the fourth melee attack once it is implemented
	*/
}

void APlayerCharacterController::ApplyInputMappingContext(TSoftObjectPtr<UInputMappingContext>& _inputMappingContext, int Priority, bool bAddContext)
{
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!_inputMappingContext.IsNull())
			{
				if (!bAddContext)
				{
					InputSystem->RemoveMappingContext(_inputMappingContext.LoadSynchronous());
				}
				else
				{
					InputSystem->AddMappingContext(_inputMappingContext.LoadSynchronous(), Priority);
				}
			}
		}
	}
}

void APlayerCharacterController::UseMeleeAttack(const FInputActionValue& Value, ESpawnableAttack::Type EMeleeAttackType)
{
	if (EInputActionValueType::Boolean == Value.GetValueType())
	{
		if (bIsMeleeAvailable)
		{
			ACharacterPawn* CharacterPawn = Cast<ACharacterPawn>(GetPawn());
			if (CharacterPawn != nullptr)
			{
				CharacterPawn->UseMeleeAttack(EMeleeAttackType);
			}
		}
	}
}
