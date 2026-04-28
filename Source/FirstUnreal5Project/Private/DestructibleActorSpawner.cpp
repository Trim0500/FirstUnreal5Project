// Fill out your copyright notice in the Description page of Project Settings.

#include "FirstUnreal5GameModeBase.h"

#include "DestructibleActorSpawner.h"

// Sets default values
ADestructibleActorSpawner::ADestructibleActorSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADestructibleActorSpawner::BeginPlay()
{
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

		ElaspedSeconds = 0.0f;
	}
}

void ADestructibleActorSpawner::Activate(bool bActivate)
{
	bActivated = bActivate;
	SetActorTickEnabled(bActivated);
	
	if (!bActivated)
	{
		ElaspedSeconds = 0.0f;
	}
}

void ADestructibleActorSpawner::OnSpawnedActorDestroyed()
{
	SpawnedActors--;
	if (SpawnedActors < MaxSpawnedActors)
	{
		Activate(true);
	}
}
