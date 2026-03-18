// Fill out your copyright notice in the Description page of Project Settings.


#include "HeavyProjectileSpawner.h"

// Sets default values
AHeavyProjectileSpawner::AHeavyProjectileSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHeavyProjectileSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHeavyProjectileSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

IProjectile* AHeavyProjectileSpawner::CreateProjectile()
{
	return nullptr;
}

