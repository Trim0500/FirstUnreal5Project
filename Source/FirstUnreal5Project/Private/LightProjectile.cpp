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

	UE_LOG(LogTemp, Warning, TEXT("[ALightProjectile::BeginPlay]: Light Projectile created..."));
	
	Launch();
}

// Called every frame
void ALightProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALightProjectile::Launch()
{
	UE_LOG(LogTemp, Warning, TEXT("[ALightProjectile::Launch]: Light Projectile will be launched..."));

	auto StaticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	if (StaticMeshComponent != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ALightProjectile::Launch]: Mesh component found, applying force..."));

		StaticMeshComponent->AddImpulse(GetActorForwardVector() * LaunchForce, NAME_None, true);

		UE_LOG(LogTemp, Warning, TEXT("[ALightProjectile::Launch]: Launch successful!"));
	}
}

void ALightProjectile::OnHitDetected()
{
	UE_LOG(LogTemp, Warning, TEXT("Light Projectile Hit Detected!"));
}
