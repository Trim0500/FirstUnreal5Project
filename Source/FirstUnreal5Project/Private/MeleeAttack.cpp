// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/CapsuleComponent.h"

#include "Constants.h"
#include "MeleeAttack.h"

using namespace Functional_Project_Constants;

// Sets default values
AMeleeAttack::AMeleeAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMeleeAttack::BeginPlay()
{
	Super::BeginPlay();

	TotalFrameTime = 0.0f;

	bPastActiveFrames = false;

	bPastRecoveryFrames = false;

	bPastCancelRecoveryFrames = false;

	SetActorHiddenInGame(true);

	SetActorEnableCollision(false);

	PlayerCharacterControllerRef = Cast<APlayerCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PlayerCharacterControllerRef != nullptr)
	{
		PlayerCharacterControllerRef->ApplyInputMappingContext(AttackInputMapping, 2, true);
	}

	if (UCapsuleComponent* AttackCapsuleComponent = Cast<UCapsuleComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass())))
	{
		AttackCapsuleComponent->SetGenerateOverlapEvents(true);

		AttackCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AMeleeAttack::OnAttackOverlapBegin);
	}
}

// Called every frame
void AMeleeAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* PlayerAttackOwner = GetOwner();
	if (ACharacterPawn* Pawn = Cast<ACharacterPawn>(PlayerAttackOwner))
	{
		TArray<UActorComponent*> CharacterMeshes = Pawn->GetComponentsByTag(UStaticMeshComponent::StaticClass(), FName(PLAYER_MESH_TAG));
		if (CharacterMeshes.Num() > 0)
		{
			UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(CharacterMeshes[0]);
			FVector MeshForwardVector = Mesh->GetForwardVector();
			
			FVector MeshLocation = Mesh->GetComponentLocation();
			FVector OffsetLocation = MeshLocation + MeshForwardVector * PLAYER_ATTACK_HITBOX_OFFSET;
			SetActorLocation(OffsetLocation);
		}
	}

	TotalFrameTime += DeltaTime;

	if (TotalFrameTime >= LifetimeFrameThreshold && bNextAttackExists)
	{
		check(PlayerCharacterControllerRef);
		
		PlayerCharacterControllerRef->ApplyInputMappingContext(NextAttackInputMapping, 0, false);

		Destroy();
	}

	if (TotalFrameTime >= CancelRecoveryFrameThreshold && !bPastCancelRecoveryFrames)
	{
		bPastCancelRecoveryFrames = true;

		check(PlayerCharacterControllerRef);

		PlayerCharacterControllerRef->ApplyInputMappingContext(AttackInputMapping, 0, false);

		if (bNextAttackExists)
		{
			PlayerCharacterControllerRef->ApplyInputMappingContext(NextAttackInputMapping, 1, true);
		}
		else
		{
			Destroy();
		}
	}

	if (TotalFrameTime >= RecoveryFrameThreshold && !bPastRecoveryFrames)
	{
		bPastRecoveryFrames = true;

		SetActorHiddenInGame(true);

		SetActorEnableCollision(false);
	}

	if (TotalFrameTime >= StartUpFrameThreshold && !bPastActiveFrames)
	{
		bPastActiveFrames = true;

		SetActorHiddenInGame(false);

		SetActorEnableCollision(true);
	}
}

void AMeleeAttack::OnAttackOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* PlayerAttackOwner = GetOwner();
	if (ACharacterPawn* Pawn = Cast<ACharacterPawn>(PlayerAttackOwner))
	{
		Pawn->CancelLunge();

		Destroy();
	}
}
