// Fill out your copyright notice in the Description page of Project Settings.

#include "FirstUnreal5GameModeBase.h"

#include "DestructibleActorSpawner.h"

// Sets default values
ADestructibleActorSpawner::ADestructibleActorSpawner()
{
	UE_LOG(LogTemp, Warning, TEXT("[ADestructibleActorSpawner::ADestructibleActorSpawner]: Function called..."));

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADestructibleActorSpawner::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("[ADestructibleActorSpawner::BeginPlay]: Function called..."));

	Super::BeginPlay();
	
	ElaspedSeconds = 0.0f;

	SpawnedActors = 0;

	Activate(bActivated);
}

// Called every frame
void ADestructibleActorSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AFirstUnreal5GameModeBase* GameMode = Cast<AFirstUnreal5GameModeBase>(GetWorld()->GetAuthGameMode());
	bool bCanSpawn = GameMode->CanSpawnEnemies();
	if (!bCanSpawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ADestructibleActorSpawner::Tick]: According to the game mode, spawning is not allowed for now..."));

		Activate(false);

		return;
	}

	ElaspedSeconds += DeltaTime;
	if (SpawnCooldown <= ElaspedSeconds)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = this;

		ADestructibleActor* SpawnedActor = GetWorld()->SpawnActor<ADestructibleActor>(SpawnedActorClass.Get(), GetActorLocation(), FRotator(), SpawnParams);
		SpawnedActor->DestroyedDelegate.AddDynamic(this, &ADestructibleActorSpawner::OnSpawnedActorDestroyed);

		SpawnedActorDelegate.Broadcast();

		SpawnedActors++;
		if (SpawnedActors >= MaxSpawnedActors)
		{
			Activate(false);
		}
	}
}

void ADestructibleActorSpawner::Activate(bool bActivate)
{
	UE_LOG(LogTemp, Warning, TEXT("[ADestructibleActorSpawner::Activate]: Function called..."));

	bActivated = bActivate;
	SetActorTickEnabled(bActivated);
	
	if (!bActivated)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ADestructibleActorSpawner::Activate]: Spawner deactivated, elapsed seconds reset..."));

		ElaspedSeconds = 0.0f;
	}
}

void ADestructibleActorSpawner::OnSpawnedActorDestroyed()
{
	UE_LOG(LogTemp, Warning, TEXT("[ADestructibleActorSpawner::OnSpawnedActorDestroyed]: Function called..."));

	SpawnedActors--;
	if (SpawnedActors < MaxSpawnedActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ADestructibleActorSpawner::OnSpawnedActorDestroyed]: Spawner reactivated, elapsed seconds reset..."));

		Activate(true);
	}
}
