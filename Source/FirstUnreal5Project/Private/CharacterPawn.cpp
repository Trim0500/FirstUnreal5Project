// Fill out your copyright notice in the Description page of Project Settings.

#define _USE_MATH_DEFINES

#include <math.h>

#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

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

	JumpGravity = (-2 * PeakJumpHeight) / FMath::Square(TimeToPeakJump);

	JumpVelocity = (2 * PeakJumpHeight) / TimeToPeakJump;

	EnableFeetOverlapEvents(false, true);
}

// Called every frame
void ACharacterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsJumping)
	{
		JumpElapsedTime += DeltaTime;
		ApplyJumpToZ(JumpGravity, JumpVelocity, JumpElapsedTime);
	}

	if (bIsJumping && TimeToPeakJump <= JumpElapsedTime)
	{
		EnableFeetOverlapEvents(true, false);
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

void ACharacterPawn::ApplyJumpToZ(float Gravity, float Velocity, float Time)
{
	auto newActorHeight = (0.5f * Gravity * (FMath::Square(Time))) + (Velocity * Time) + JumpStartZ;
	AddMovementInput(GetActorUpVector(), newActorHeight);
}

void ACharacterPawn::JumpPawn()
{
	bIsJumping = true;

	JumpElapsedTime = 0.0f;

	EnableGravity(false);

	JumpStartZ = GetActorLocation().Z;
}

void ACharacterPawn::FireProjectile(ProjectileType ProjectileType)
{
	auto ProjectileClass = ProjectileType == ProjectileType::Light ? LightProjectileClass : HeavyProjectileClass;
	if (ProjectileClass != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = this;

		FVector SpawnLocation;
		
		FVector MeshForwardVector;

		FRotator MeshRotation;

		TArray<UActorComponent*> Mesh = GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("PlayerMesh"));
		if (Mesh.Num() > 0)
		{
			auto StaticMeshComponent = Cast<UStaticMeshComponent>(Mesh[0]);
			SpawnLocation = StaticMeshComponent->GetComponentLocation();

			MeshForwardVector = StaticMeshComponent->GetForwardVector();
			SpawnLocation += MeshForwardVector * 10.0f;

			MeshRotation = StaticMeshComponent->GetComponentRotation();
			GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, MeshRotation, SpawnParams);
		}
	}
}

void ACharacterPawn::UseMeleeAttack(MeleeAttackType MeleeAttackType)
{
	/*
	* [PC-05]: TODO
	*			Implement melee attack functionality.
	*
	*			This will likely involve spawning a hitbox in front of the character that can interact with enemies and other objects.
	*			
	*			The hitbox will be active for a short duration and will be spawned in front of the character based on the character's forward vector.
	*/
}

void ACharacterPawn::OnFeetOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor->ActorHasTag("Floor"))
	{
		EnableFeetOverlapEvents(false, false);

		JumpStartZ = 0.0f;

		EnableGravity(true);

		JumpElapsedTime = 0.0f;

		bIsJumping = false;

		Cast<APlayerCharacterController>(GetController())->bIsJumpAvailable = true;
	}
}

void ACharacterPawn::EnableGravity(bool bEnable)
{
	auto rootComponent = RootComponent;
	if (auto rootPrimitive = Cast<UPrimitiveComponent>(rootComponent))
	{
		rootPrimitive->SetEnableGravity(bEnable);
	}
}

void ACharacterPawn::EnableFeetOverlapEvents(bool enable, bool mapOverlapFunction)
{
	auto groundDetectorComponents = GetComponentsByTag(UPrimitiveComponent::StaticClass(), FName("GroundDetector"));
	if (groundDetectorComponents.Num() > 0)
	{
		if (auto groundDetectorPtr = Cast<UBoxComponent>(groundDetectorComponents[0]))
		{
			groundDetectorPtr->SetGenerateOverlapEvents(enable);

			if (mapOverlapFunction)
			{
				groundDetectorPtr->OnComponentBeginOverlap.AddDynamic(this, &ACharacterPawn::OnFeetOverlapBegin);
			}
		}
	}
}
