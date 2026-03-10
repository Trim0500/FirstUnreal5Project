// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"

#include "PlayerCharacterController.h"
#include "Constants.h"

using namespace Functional_Project_Constants;

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent != nullptr);

	// TODO [PC-06]
	/*
	*	Refactor to use the ApplyInputMappingContext function and map the neutral input mapping context to the player character controller
	*
	*	Change priority of neutral input mapping context to be lower than the priority of the combat input mapping context
	*		Keep the priority in a new constants file and use it in both the player character controller and the input mapping contexts
	*/
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
		EnhancedInputComponenet->BindAction(MeleeAttackThree, ETriggerEvent::Triggered, this, &APlayerCharacterController::EnhancedUseMeleeAttackThree);
		EnhancedInputComponenet->BindAction(MeleeAttackFour, ETriggerEvent::Triggered, this, &APlayerCharacterController::EnhancedUseMeleeAttackFour);
		EnhancedInputComponenet->BindAction(BeginLockOn, ETriggerEvent::Triggered, this, &APlayerCharacterController::EnhancedBeginLockOn);

		// TODO [PC-06]
		/*
		*	Map input actions for dodge, lunge and lock-on state changes
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
	UseAttack(value, ESpawnableAttack::ProjectileLight);
}

void APlayerCharacterController::EnhancedFireHeavyProjectile(const FInputActionValue& value)
{
	UseAttack(value, ESpawnableAttack::ProjectileHeavy);
}

void APlayerCharacterController::EnhancedUseMeleeAttackOne(const FInputActionValue& value)
{
	UseAttack(value, ESpawnableAttack::MeleeOne);
}

void APlayerCharacterController::EnhancedUseMeleeAttackTwo(const FInputActionValue& value)
{
	UseAttack(value, ESpawnableAttack::MeleeTwo);
}

void APlayerCharacterController::EnhancedUseMeleeAttackThree(const FInputActionValue& value)
{
	UseAttack(value, ESpawnableAttack::MeleeThree);
}

void APlayerCharacterController::EnhancedUseMeleeAttackFour(const FInputActionValue& value)
{
	UseAttack(value, ESpawnableAttack::MeleeFour);
}

void APlayerCharacterController::EnhancedUseDodge(const FInputActionValue& value)
{
	// TODO [PC-06]
	/*
	*	Implement dodge action input handling
	* 
	*	Use the character pawn's dodge function to execute the dodge action
	*/
}

void APlayerCharacterController::EnhancedUseLunge(const FInputActionValue& value)
{
	// TODO [PC-06]
	/*
	*	Implement lunge action input handling
	* 
	*	Use the character pawn's Attack function to execute the lunge action
	*/
}

void APlayerCharacterController::EnhancedBeginLockOn(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Warning, TEXT("[APlayerCharacterController::EnhancedBeginLockOn]: function called..."));

	if (EInputActionValueType::Boolean == value.GetValueType())
	{
		UE_LOG(LogTemp, Warning, TEXT("[APlayerCharacterController::EnhancedBeginLockOn]: input accepted..."));

		ApplyInputMappingContext(LockOnInputMapping, LOCKON_INPUT_MAPPING_PRIORITY, true);

		ACharacterPawn* CharacterPawn = Cast<ACharacterPawn>(GetPawn());
		if (CharacterPawn != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[APlayerCharacterController::EnhancedBeginLockOn]: Found pawn! Calling ToggleLockOn..."));

			CharacterPawn->ToggleLockOn(true);
		}
	}
}

void APlayerCharacterController::EnhancedEndLockOn(const FInputActionValue& value)
{
	// TODO [PC-06]
	/*
	*	Implement end lock-on action input handling
	* 
	*	Revoke the lock-on mapping context with ApplyInputMappingContext
	*		Establish the priority in a constants file and use that variable when passing priority argument
	* 
	*	Use the character pawn's respective function to clear out the target list and exit lock-on state
	*/
}

void APlayerCharacterController::EnhancedChangeLockOnTarget(const FInputActionValue& value)
{
	// TODO [PC-06]
	/*
	*	Implement change lock-on target action input handling
	* 
	*	Use the character pawn's respective function to recalculate the lock-on target list and change the current lock-on target to the next one in the list
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

void APlayerCharacterController::UseAttack(const FInputActionValue& Value, ESpawnableAttack::EType EAttackType)
{
	if (EInputActionValueType::Boolean == Value.GetValueType())
	{
		if (EAttackType < ESpawnableAttack::MeleeOne || bIsMeleeAvailable)
		{
			ACharacterPawn* CharacterPawn = Cast<ACharacterPawn>(GetPawn());
			if (CharacterPawn != nullptr)
			{
				CharacterPawn->Attack(EAttackType);
			}
		}
	}
}
