// Fill out your copyright notice in the Description page of Project Settings.


#include "LightProjectileSpawner.h"

// Sets default values
ALightProjectileSpawner::ALightProjectileSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALightProjectileSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALightProjectileSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

IProjectile* ALightProjectileSpawner::CreateProjectile()
{
	return nullptr;
}
