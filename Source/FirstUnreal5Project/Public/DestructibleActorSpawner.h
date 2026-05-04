// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DestructibleActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleActorSpawner.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpawnedActorDelegate, ADestructibleActor*, SpawnedActor);

UCLASS()
class FIRSTUNREAL5PROJECT_API ADestructibleActorSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Parameters")
	float SpawnCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Parameters")
	TSoftClassPtr<ADestructibleActor> SpawnedActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Parameters")
	bool bActivated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner Parameters")
	int MaxSpawnedActors;

	FOnSpawnedActorDelegate SpawnedActorDelegate;

	// Sets default values for this actor's properties
	ADestructibleActorSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Activate(bool);

	UFUNCTION()
	void OnSpawnedActorDestroyed();

private:
	float ElaspedSeconds;

	int SpawnedActors;
};
