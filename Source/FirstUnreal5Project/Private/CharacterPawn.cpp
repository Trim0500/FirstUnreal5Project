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

	for (const FAttackInfo& Info : AttackInfoArray)
	{
		if (Info.AttackClass != nullptr && !AttackMap.Contains(Info.AttackType))
		{
			AttackMap.Add(Info.AttackType, Info.AttackClass);

			switch (Info.AttackType)
			{
				case ESpawnableAttack::MeleeOne:
					MeleeAttackRotatorMap.Add(Info.AttackType, FRotator(0.0f, 0.0f, 45.0f));
					
					break;

				case ESpawnableAttack::MeleeTwo:
					MeleeAttackRotatorMap.Add(Info.AttackType, FRotator(0.0f, 0.0f, 90.0f));
					
					break;
				case ESpawnableAttack::MeleeThree:
					MeleeAttackRotatorMap.Add(Info.AttackType, FRotator(0.0f, 0.0f, 150.0f));
					
					break;
				case ESpawnableAttack::MeleeFour:
					MeleeAttackRotatorMap.Add(Info.AttackType, FRotator(0.0f, 0.0f, 180.0f));
					
					break;
				default:
					break;
			}
		}
	}

	/*
	* [PC-06]: TODO
	*			Set current lock-on target index to 0
	*			Set lock-on active to false
	*/
}

// Called every frame
void ACharacterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	* [PC-06]: TODO
	*			Assuming that lock-on is active need to do the following
	*				Call ApplyLockOnRotation
	*				Call AdjustCameraForLockOn
	*/

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

void ACharacterPawn::Attack(ESpawnableAttack::EType EAttackType)
{
	if (AttackMap.Contains(EAttackType))
	{
		SpawnAttack(EAttackType, 50.f, EAttackType > ESpawnableAttack::ProjectileHeavy ? MeleeAttackRotatorMap[EAttackType] : FRotator());
	}
}

void ACharacterPawn::ToggleLockOn(bool bActivateLockOn)
{
	/*
	* [PC-06]: TODO
	*			Implement logic for character pawn to toggle lock-on state when lock-on is enabled/disabled 
	*				If enabled:
	*					Set lock-on active boolean to true
	*					Calculate lock-on targets to populate array
	*					Select first lock-on target from array as current lock-on target
	*					Keep track of the current lock-on target index in the array to allow for cycling through targets when lock-on is active
	* 
	*				If disabled:
	*					Reset the lock-on target index
	*					Clear lock-on target
	*					Clear lock-on target array
	*					Set lock-on flag to false
	*/
}

void ACharacterPawn::CycleLockOnTarget()
{
	/*
	* [PC-06]: TODO
	* 			Implement logic for character pawn to cycle through lock-on targets
	* 
	*			Calculate lock-on targets, passing over current lock-on target and all others that came before
	*			Set next target in array as current lock-on target
	*			Increment lock-on target index, use modulo with length of lock-on target array to loop back to beginning of array if index exceeds array length
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

		auto PlayerController = Cast<APlayerCharacterController>(GetController());
		PlayerController->bIsJumpAvailable = true;
		PlayerController->bIsMeleeAvailable = true;
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

void ACharacterPawn::SpawnAttack(ESpawnableAttack::EType AttackType, float SpawnDistance, FRotator Rotator)
{
	if (TSubclassOf<AActor>* AttackClass = AttackMap.Find(AttackType))
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
			SpawnLocation += MeshForwardVector * SpawnDistance;
			
			MeshRotation = StaticMeshComponent->GetComponentRotation();
			GetWorld()->SpawnActor<AActor>(*AttackClass, SpawnLocation, MeshRotation + Rotator, SpawnParams);
		}
	}
}

void ACharacterPawn::ApplyLockOnRotation()
{
	/*
	* [PC-06]: TODO
	*			Implement lock-on rotation logic to rotate the player to face the current lock-on target when lock-on is active
	* 
	*			Use the current lock-on target's location and the player's location and determine the rotation that must be applied to the player to face the lock-on target
	*			Once calculated, add the result to the pawn's current rotation to rotate the player to face the lock-on target
	*/			
}

void ACharacterPawn::CalculateLockOnTargets(bool bCycleTriggered)
{
	/*
	* [PC-06]: TODO
	*			Implement lock-on target calculation logic to populate array of lock-on targets when lock-on is enabled and when cycling through lock-on targets
	* 
	*			If target cycling was triggered:
	*				Grab current lock-on target reference by index and all previous targets in the lock-on target array
	*				Begin creation of new lock-on target array starting with previous lock-on targets up to and including current lock-on target
	*				Recalculate new lock-on targets by locating actors with valid tag and within maxmimum target range
	*				Append list
	* 
	*			If target cycling was not triggered:
	*				Recalculate new lock-on targets by locating actors with valid tag and within maxmimum target range
	*/
}

void ACharacterPawn::AdjustCameraForLockOn()
{
	/*
	* [PC-06]: TODO
	*			Implement camera adjustment logic to adjust the camera position and rotation when lock-on is active to better frame the current lock-on target
	* 
	*			Find the current lock-on target's location by querying lock-on target array with current lock-on target index
	*			Use midpoint formula to find the midpoint between the player and the current lock-on target in world space
	* 			Set the camera's location to the midpoint location, maintaining the camera's current height
	*/
}
