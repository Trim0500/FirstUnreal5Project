// Fill out your copyright notice in the Description page of Project Settings.

#define _USE_MATH_DEFINES

#include <math.h>

#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

#include "CharacterPawn.h"
#include "Constants.h"
#include "PlayerCharacterController.h"

using namespace Functional_Project_Constants;

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
				case ESpawnableAttack::Lunge:
					MeleeAttackRotatorMap.Add(Info.AttackType, FRotator(270.0f, 0.0f, 0.0f));

					break;
				default:
					break;
			}
		}
	}

	CurrentLockOnTargetIndex = 0;

	bIsLockOnActive = false;

	LockOnTargets.Empty();

	bIsDodging = false;

	CurrentDodgeInfo = FDodgeInfo();
}

// Called every frame
void ACharacterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDodging)
	{
		CurrentDodgeInfo.ElapsedTime += DeltaTime;
		if (CurrentDodgeInfo.ElapsedTime >= CurrentDodgeInfo.MaxDodgeTime)
		{
			CancelLunge();
		}
		else
		{
			ApplyDodge(CurrentDodgeInfo);
		}
	}

	if (bIsLockOnActive)
	{
		FVector LockOnTargetLocation = LockOnTargets[CurrentLockOnTargetIndex].TargetTransform;
		FVector LocationDifference = LockOnTargets[CurrentLockOnTargetIndex].TargetTransform - GetActorLocation();
		SetPawnMeshRotator(LocationDifference.Y, LocationDifference.X);

		AdjustCameraForLockOn(LockOnTargetLocation);
	}

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

		if (!bIsLockOnActive)
		{
			SetPawnMeshRotator(PotentialMovementVector.Y, PotentialMovementVector.X);

			AdjustCameraForLockOn(GetActorLocation());
		}

		FVector NewMovementVector = GetActorLocation();

		if (bIsDodging)
		{
			NewMovementVector.X = PotentialMovementVector.X;
			NewMovementVector.Y = PotentialMovementVector.Y;
		}
		else
		{
			NewMovementVector.X += PotentialMovementVector.X * MoveScale;
			NewMovementVector.Y += PotentialMovementVector.Y * MoveScale;
		}

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

		float moveScale = bIsLockOnActive && !bIsJumping ? MoveScale * InputVector * LockOnMoveReductionScale : MoveScale * InputVector;
		AddMovementInput(CameraRightVector, moveScale);
	}
}

void ACharacterPawn::MovePawnVertically(float InputVector)
{
	InputVector = FMath::Clamp(InputVector, -1.0f, 1.0f);
	auto PlayerCamera = GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("CameraSwivel"));
	if (PlayerCamera.Num() > 0)
	{
		auto CameraForwardVector = Cast<UStaticMeshComponent>(PlayerCamera[0])->GetForwardVector();
		float moveScale = bIsLockOnActive && !bIsJumping ? MoveScale * InputVector * LockOnMoveReductionScale : MoveScale * InputVector;
		AddMovementInput(CameraForwardVector, moveScale);
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
		SpawnAttack(EAttackType, PLAYER_ATTACK_HITBOX_OFFSET, EAttackType > ESpawnableAttack::ProjectileHeavy ? MeleeAttackRotatorMap[EAttackType] : FRotator());
	}
}

void ACharacterPawn::ToggleLockOn(bool bActivateLockOn)
{
	if (bActivateLockOn)
	{
		bIsLockOnActive = true;

		CalculateLockOnTargets(false);

		CurrentLockOnTargetIndex = 0;
	}
	else
	{
		CurrentLockOnTargetIndex = 0;

		LockOnTargets.Empty();

		bIsLockOnActive = false;

		AdjustCameraForLockOn(GetActorLocation());
	}
}

void ACharacterPawn::CycleLockOnTarget()
{
	CalculateLockOnTargets(true);

	CurrentLockOnTargetIndex = (CurrentLockOnTargetIndex + 1) % LockOnTargets.Num();
}

void ACharacterPawn::Dodge(bool bUseLastInputVector)
{
	TArray<UActorComponent*> Meshes = GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("PlayerMesh"));
	FVector DirectionVector = bUseLastInputVector
								? GetLastMovementInputVector()
								: Meshes.Num() > 0
									? Cast<UStaticMeshComponent>(Meshes[0])->GetForwardVector()
									: FVector();
	CurrentDodgeInfo = FDodgeInfo(GetActorLocation()
									, bUseLastInputVector ? DodgeDistanceScale : LungeDistanceScale
									, DirectionVector
									, 0.0f
									, bUseLastInputVector ? MaxDodgeTime : MaxLungeTime
									, bUseLastInputVector);

	bIsDodging = true;

	Cast<APlayerCharacterController>(GetController())->ApplyInputMappingContext(DodgeInputMapping, DODGE_INPUT_MAPPING_PRIORITY, true);

	SetActorEnableCollision(false);

	EnableGravity(false);
}

void ACharacterPawn::Lunge()
{
	// TODO [PC-06]
	/*
	*	Implement the public API to begin a lunge action in the direction of the current lock-on target
	* 
	*	Initialize current lunge info struct with the pawn's mesh forward vector as the direction to lunge in
	* 
	*	Set dodge flag to true to trigger lunge movement in Tick function
	* 
	*	Make the actor intangible by calling SetActorEnableCollision(false) so that the player can lunge through enemies and other obstacles
	* 
	*	Disable gravity during the lunge by calling EnableGravity(false) so that the lunge movement is not affected by gravity
	* 
	*	Call Attack passing over the lunge attack type to trigger the lunge attack hitbox to spawn during the lunge movement
	*		NOTE: The lunge attack hitbox should be set to spawn at the player's location and should move with the player during the lunge movement, so that it can hit enemies that are in the way of the lunge
	*				This may be acheived by making a subsclass of the attack actor class whereby when its tick function is called, it sets its location to be the same as the player's location
	*/

	Dodge(false);

	Attack(ESpawnableAttack::Lunge);
}

void ACharacterPawn::CancelLunge()
{
	EnableGravity(true);

	SetActorEnableCollision(true);

	Cast<APlayerCharacterController>(GetController())->ApplyInputMappingContext(DodgeInputMapping, DODGE_INPUT_MAPPING_PRIORITY, false);

	bIsDodging = false;

	CurrentDodgeInfo = FDodgeInfo();
}

float ACharacterPawn::EaseOut(float Time)
{
	return 1 <= Time ? 1 : 1 - FMath::Pow(2, -10 * Time);
}

FVector ACharacterPawn::GetDodgeLocation(FDodgeInfo DodgeInfo)
{
	float EaseOutTime = DodgeInfo.ElapsedTime / DodgeInfo.MaxDodgeTime;
	float EaseOutScale = DodgeInfo.bUseEaseOut ? EaseOut(EaseOutTime) : EaseOutTime;
	float LocationScale = DodgeInfo.DodgeDistanceScale * EaseOutScale;

	return DodgeInfo.StartLocation + DodgeInfo.TargetDirection * LocationScale;
}

void ACharacterPawn::ApplyDodge(FDodgeInfo DodgeInfo)
{
	FVector NextDodgeLocation = GetDodgeLocation(DodgeInfo);
	AddMovementInput(NextDodgeLocation);
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

void ACharacterPawn::SetPawnMeshRotator(float Adjacent, float Opposite)
{
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

void ACharacterPawn::CalculateLockOnTargets(bool bCycleTriggered)
{
	FVector ActorLocation = GetActorLocation();

	TMap<AActor*, bool> PreviousLockOnTargetMap;

	TArray<LockOnTargetInfo> PreviousLockOnTargetArray;

	if (bCycleTriggered)
	{
		for (int i = 0; i <= CurrentLockOnTargetIndex; i++)
		{
			PreviousLockOnTargetMap.Add(LockOnTargets[i].LockOnCandidate, true);
			
			PreviousLockOnTargetArray.Add(LockOnTargets[i]);
		}
	}
	
	LockOnTargets = PreviousLockOnTargetArray;

	TArray<LockOnTargetInfo> NewPotentialLockOnTargets;

	TArray<AActor*> NewPotentialTargets;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(LOCK_ON_TARGET_TAG), NewPotentialTargets);

	for (AActor* Target : NewPotentialTargets)
	{
		if (PreviousLockOnTargetMap.Contains(Target))
		{
			continue;
		}

		FVector TargetLocation = Target->GetActorLocation();
		float DistanceToTarget = FVector::Dist(TargetLocation, ActorLocation);
		if (DistanceToTarget <= MaxLockOnDistance)
		{
			NewPotentialLockOnTargets.Add(LockOnTargetInfo(Target, TargetLocation));
		}
	}

	NewPotentialLockOnTargets.Sort([ActorLocation](const LockOnTargetInfo& A, const LockOnTargetInfo& B) {
		float DistanceA = FVector::Dist(A.TargetTransform, ActorLocation);

		float DistanceB = FVector::Dist(B.TargetTransform, ActorLocation);
		
		return DistanceA < DistanceB;
	});

	for (int i = 0; i < NewPotentialLockOnTargets.Num(); i++)
	{
		LockOnTargets.Add(NewPotentialLockOnTargets[i]);
	}
}

void ACharacterPawn::AdjustCameraForLockOn(FVector TargetLocation)
{
	FVector WorldMidpointLocation = (GetActorLocation() + TargetLocation) / 2;

	TArray<UActorComponent*> MeshComponents = GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName("CameraSwivel"));
	if (MeshComponents.Num() > 0)
	{
		UStaticMeshComponent* CameraSwivel = Cast<UStaticMeshComponent>(MeshComponents[0]);
		FVector CurrentCameraLocation = CameraSwivel->GetComponentLocation();

		FVector NewCameraLocation = FVector(WorldMidpointLocation.X, WorldMidpointLocation.Y, CurrentCameraLocation.Z);
		CameraSwivel->SetWorldLocation(NewCameraLocation);
	}
}
