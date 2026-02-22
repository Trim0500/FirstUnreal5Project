// Fill out your copyright notice in the Description page of Project Settings.


#include "LightProjectile.h"

// Sets default values
ALightProjectile::ALightProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DestroyDelay = 1.0f;
}

// Called when the game starts or when spawned
void ALightProjectile::BeginPlay()
{
	Super::BeginPlay();

	auto StaticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	if (StaticMeshComponent != nullptr)
	{
		StaticMeshComponent->OnComponentHit.AddDynamic(this, &ALightProjectile::OnHitDetected);
	}
	
	Launch();
}

// Called every frame
void ALightProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DestroyElapsedTime += DeltaTime;
	if (DestroyElapsedTime >= DestroyDelay)
	{
		Destroy();
	}
}

void ALightProjectile::Launch()
{
	auto StaticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	if (StaticMeshComponent != nullptr)
	{
		StaticMeshComponent->AddImpulse(GetActorForwardVector() * LaunchForce, NAME_None, true);
	}
}

void ALightProjectile::OnHitDetected(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}
