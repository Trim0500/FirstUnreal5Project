// Fill out your copyright notice in the Description page of Project Settings.

#define _USE_MATH_DEFINES

#include <math.h>

#include "Kismet/GameplayStatics.h"

#include "CharacterPawn.h"
#include "PlayerCharacterController.h"

// Sets default values
ACharacterPawn::ACharacterPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACharacterPawn::BeginPlay()
{
	Super::BeginPlay();

	/*
	* NOTE: Unsure how to set up the collision component to detect collision with the ground. Will need to research this further.
	*/
	/*if (Feet)
	{
		Feet->OnComponentBeginOverlap.AddDynamic(this, &ACharacterPawn::OnFeetOverlapBegin);
	}*/
}

// Called every frame
void ACharacterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	* [PC-02]: TODO: Add logic to handle jumping.
	*			Handling the logic should be done via a function ( AddJumpToZ ) that passes over current velocity and acceleration values.
	*/
	if (_isJumping)
	{
		ApplyJumpToZ(_currentJumpVelocity, JumpAcceleration, DeltaTime);
	}

	/*
	* [PC-02]: TODO: Add logic to detect when _currentJumpVelocity is negative.
	*					If check passes, activate a component that detects collision with the ground and resets _isJumping to false and _currentJumpVelocity
	*					to InitJumpVelocity.
	*					Should try to find the component by using GetComponentsByTag and searching for a component with tag "GroundDetector".
	*/
	if (_currentJumpVelocity <= 0)
	{
		/*
		* NOTE: Unsure how to set up the collision component to detect collision with the ground.Will need to research this further.
		*		For now, will just reset the jump values to allow for infinite jumping.
		*/
		_isJumping = false;

		_currentJumpVelocity = InitJumpVelocity;

		Cast<APlayerCharacterController>(GetController())->IsJumpAvailable = true;
	}

	FVector PotentialMovementVector = ConsumeMovementInputVector();
	if (PotentialMovementVector.X != 0 || PotentialMovementVector.Y != 0 || PotentialMovementVector.Z != 0)
	{
		// Need to update the rotation of the root object along with the character mesh
		// Use Pythagreum threorm to determine the target yaw angle that must be set.
		// Ex. Input is received as [1, 0] which based on plane for object is X = 0 and Y = 1
		// With a counter clockwise rotation, this means the angle is 90

		float Adjacent = PotentialMovementVector.Y;
		float Opposite = PotentialMovementVector.X;

		float QuadrantValue = 0.0f;
		if (Adjacent <= 0 && Opposite >= 0)
		{
			QuadrantValue = 270.0;
		}
		else if (Adjacent <= 0 && Opposite <= 0)
		{
			float Temp = Adjacent;
			Adjacent = Opposite;
			Opposite = Temp;

			QuadrantValue = 180.0;
		}
		else if (Adjacent >= 0 && Opposite <= 0)
		{
			QuadrantValue = 90.0;
		}
		else
		{
			float Temp = Adjacent;
			Adjacent = Opposite;
			Opposite = Temp;
		}

		float Hypoteneuse = FMath::Sqrt(FMath::Square(Adjacent) + FMath::Square(Opposite));

		float Radian = FMath::Asin(FMath::Abs(Opposite) / Hypoteneuse);

		float Degree = Radian * (180 / M_PI) + QuadrantValue;

		TArray<UActorComponent*> Mesh = GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("PlayerMesh"));
		if (Mesh.Num() > 0)
		{
			FRotator NewMeshRotation = FRotator(0.0f, Degree, 0.0f);
			Cast<UStaticMeshComponent>(Mesh[0])->SetRelativeRotation(NewMeshRotation);
		}

		FVector NewMovementVector = GetActorLocation();
		NewMovementVector.X += PotentialMovementVector.X * MoveScale;
		NewMovementVector.Y += PotentialMovementVector.Y * MoveScale;
		/*
		* [PC-02]: TODO: Remove change in Z axis through multiplying the Z value of the PotentialMovementVector by JumpScale and
							adding it to the current Z value of the NewMovementVector.
		*/
		NewMovementVector.Z += PotentialMovementVector.Z;
		SetActorLocation(NewMovementVector);
	}
}

void ACharacterPawn::MovePawnHorizontally(float _inputVector)
{
	_inputVector = FMath::Clamp(_inputVector, -1.0f, 1.0f);
	auto PlayerCamera = GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("CameraSwivel"));
	if (PlayerCamera.Num() > 0)
	{
		auto CameraRightVector = Cast<UStaticMeshComponent>(PlayerCamera[0])->GetRightVector();
		AddMovementInput(CameraRightVector, MoveScale * _inputVector);
	}
}

void ACharacterPawn::MovePawnVertically(float _inputVector)
{
	_inputVector = FMath::Clamp(_inputVector, -1.0f, 1.0f);
	auto PlayerCamera = GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("CameraSwivel"));
	if (PlayerCamera.Num() > 0)
	{
		auto CameraForwardVector = Cast<UStaticMeshComponent>(PlayerCamera[0])->GetForwardVector();
		AddMovementInput(CameraForwardVector, MoveScale * _inputVector);
	}
}

/*
* [PC-02]: TODO: Add in function AddJumpToZ.
*					Params: currentJumpVelocity ( float )
*							acceleration ( float )
* 					Return: void
*					Void function that uses AddMovementInput to apply the movement vector to the character.
*					Should only apply movement to Z axis.
*					Update _currentJumpVelocity by adding the acceleration value.
*/
void ACharacterPawn::ApplyJumpToZ(float currentJumpVelocity, float acceleration, float deltaTime)
{
	auto currentActorZValue = GetActorLocation().Z;
	currentActorZValue += (currentJumpVelocity * deltaTime) + (0.5f * acceleration * (FMath::Square(deltaTime)));
	
	currentJumpVelocity += acceleration * deltaTime;

	AddMovementInput(GetActorUpVector(), currentActorZValue);

	_currentJumpVelocity = currentJumpVelocity;
}

void ACharacterPawn::JumpPawn()
{
	/*
	* [PC-02]: TODO: Change functionality to simply set _isJumping to true and set _currentJumpVelocity to InitJumpVelocity.
	*/
	_isJumping = true;

	_currentJumpVelocity = InitJumpVelocity;
}

/*
* NOTE: Unsure how to set up the collision component to detect collision with the ground. Will need to research this further.
*/
//void ACharacterPawn::OnFeetOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//
//}
