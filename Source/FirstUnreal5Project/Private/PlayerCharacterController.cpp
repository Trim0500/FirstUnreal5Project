// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"

#include "PlayerCharacterController.h"
#include "Constants.h"

using namespace Functional_Project_Constants;

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent != nullptr);

	ApplyInputMappingContext(InputMapping, DEFAULT_INPUT_MAPPING_PRIORITY, true);

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
		EnhancedInputComponenet->BindAction(EndLockOn, ETriggerEvent::Triggered, this, &APlayerCharacterController::EnhancedEndLockOn);
		EnhancedInputComponenet->BindAction(ChangeLockOnTarget, ETriggerEvent::Triggered, this, &APlayerCharacterController::EnhancedChangeLockOnTarget);
		EnhancedInputComponenet->BindAction(Dodge, ETriggerEvent::Triggered, this, &APlayerCharacterController::EnhancedUseDodge);
		EnhancedInputComponenet->BindAction(Lunge, ETriggerEvent::Triggered, this, &APlayerCharacterController::EnhancedUseLunge);
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
	if (EInputActionValueType::Boolean == value.GetValueType())
	{
		ACharacterPawn* CharacterPawn = Cast<ACharacterPawn>(GetPawn());
		if (CharacterPawn != nullptr)
		{
			CharacterPawn->Dodge(true);
		}
	}
}

void APlayerCharacterController::EnhancedUseLunge(const FInputActionValue& value)
{
	if (EInputActionValueType::Boolean == value.GetValueType())
	{
		ACharacterPawn* CharacterPawn = Cast<ACharacterPawn>(GetPawn());
		if (CharacterPawn != nullptr)
		{
			TArray<UActorComponent*> Meshes = CharacterPawn->GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("PlayerMesh"));
			if (Meshes.Num() > 0)
			{
				FVector PlayerMeshForwardVector = Cast<UStaticMeshComponent>(Meshes[0])->GetForwardVector();

				FVector LastPawnInputVector = CharacterPawn->GetLastMovementInputVector();
				FVector VectorDirectionDifference = LastPawnInputVector - PlayerMeshForwardVector;
				float absoluteMagnitudeDifference = VectorDirectionDifference.Size2D();
				if (absoluteMagnitudeDifference <= 0.5)
				{
					CharacterPawn->Dodge(false);
				}
				else
				{
					CharacterPawn->Attack(ESpawnableAttack::MeleeOne);
				}
			}
		}
	}
}

void APlayerCharacterController::EnhancedBeginLockOn(const FInputActionValue& value)
{
	if (EInputActionValueType::Boolean == value.GetValueType())
	{
		ApplyInputMappingContext(LockOnInputMapping, LOCKON_INPUT_MAPPING_PRIORITY, true);

		ACharacterPawn* CharacterPawn = Cast<ACharacterPawn>(GetPawn());
		if (CharacterPawn != nullptr)
		{
			CharacterPawn->ToggleLockOn(true);
		}
	}
}

void APlayerCharacterController::EnhancedEndLockOn(const FInputActionValue& value)
{
	if (EInputActionValueType::Boolean == value.GetValueType())
	{
		ApplyInputMappingContext(LockOnInputMapping, LOCKON_INPUT_MAPPING_PRIORITY, false);

		ACharacterPawn* CharacterPawn = Cast<ACharacterPawn>(GetPawn());
		if (CharacterPawn != nullptr)
		{
			CharacterPawn->ToggleLockOn(false);
		}
	}
}

void APlayerCharacterController::EnhancedChangeLockOnTarget(const FInputActionValue& value)
{
	if (EInputActionValueType::Boolean == value.GetValueType())
	{
		ACharacterPawn* CharacterPawn = Cast<ACharacterPawn>(GetPawn());
		if (CharacterPawn != nullptr)
		{
			CharacterPawn->CycleLockOnTarget();
		}
	}
}

void APlayerCharacterController::ApplyInputMappingContext(TSoftObjectPtr<UInputMappingContext>& _inputMappingContext, int Priority, bool bAddContext)
{
	APlayerController::FlushPressedKeys();

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
