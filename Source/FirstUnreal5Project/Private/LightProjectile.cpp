// Fill out your copyright notice in the Description page of Project Settings.


#include "LightProjectile.h"

// Sets default values
ALightProjectile::ALightProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALightProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALightProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALightProjectile::Launch()
{
	UE_LOG(LogTemp, Warning, TEXT("Light Projectile Launched!"));
}

void ALightProjectile::OnHitDetected()
{
	UE_LOG(LogTemp, Warning, TEXT("Light Projectile Hit Detected!"));
}
