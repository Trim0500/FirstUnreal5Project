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

	if (bIsJumping)
	{
		JumpElapsedTime += DeltaTime;
		ApplyJumpToZ(JumpVelocity, JumpGravity, JumpElapsedTime);
	}

	if (JumpElapsedTime >= 0.45)
	{
		/*
		* NOTE: Unsure how to set up the collision component to detect collision with the ground. Will need to research this further.
		*		For now, will base jump reset on timer based on expected jump duration. Will need to adjust this value based on testing and how it feels in game.
		*/

		JumpStartZ = 0.0f;

		EnableGravity(true);

		JumpElapsedTime = 0.0f;

		bIsJumping = false;

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
		if (bIsJumping)
		{
			NewMovementVector.Z = PotentialMovementVector.Z;
		}
		SetActorLocation(NewMovementVector);
	}
}

void ACharacterPawn::MovePawnHorizontally(float InputVector)
{
	InputVector = FMath::Clamp(InputVector, -1.0f, 1.0f);
	auto PlayerCamera = GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("CameraSwivel"));
	if (PlayerCamera.Num() > 0)
	{
		auto CameraRightVector = Cast<UStaticMeshComponent>(PlayerCamera[0])->GetRightVector();
		AddMovementInput(CameraRightVector, MoveScale * InputVector);
	}
}

void ACharacterPawn::MovePawnVertically(float InputVector)
{
	InputVector = FMath::Clamp(InputVector, -1.0f, 1.0f);
	auto PlayerCamera = GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("CameraSwivel"));
	if (PlayerCamera.Num() > 0)
	{
		auto CameraForwardVector = Cast<UStaticMeshComponent>(PlayerCamera[0])->GetForwardVector();
		AddMovementInput(CameraForwardVector, MoveScale * InputVector);
	}
}

void ACharacterPawn::ApplyJumpToZ(float Velocity, float Gravity, float Time)
{
	auto newActorHeight = JumpStartZ + (Velocity * Time) + (0.5f * Gravity * (FMath::Square(Time)));
	AddMovementInput(GetActorUpVector(), newActorHeight);
}

void ACharacterPawn::JumpPawn()
{
	bIsJumping = true;

	JumpElapsedTime = 0.0f;

	EnableGravity(false);

	JumpStartZ = GetActorLocation().Z;
}

/*
* NOTE: Unsure how to set up the collision component to detect collision with the ground. Will need to research this further.
*/
//void ACharacterPawn::OnFeetOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//
//}

void ACharacterPawn::EnableGravity(bool bEnable)
{
	auto rootComponent = RootComponent;
	if (auto rootPrimitive = Cast<UPrimitiveComponent>(rootComponent))
	{
		rootPrimitive->SetEnableGravity(bEnable);
	}
}
