// Fill out your copyright notice in the Description page of Project Settings.


#include "HeavyProjecile.h"

// Sets default values
AHeavyProjecile::AHeavyProjecile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DestroyDelay = 1.0f;
}

// Called when the game starts or when spawned
void AHeavyProjecile::BeginPlay()
{
	Super::BeginPlay();
	
	auto StaticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	if (StaticMeshComponent != nullptr)
	{
		StaticMeshComponent->OnComponentHit.AddDynamic(this, &AHeavyProjecile::OnHitDetected);
	}

	Launch();
}

// Called every frame
void AHeavyProjecile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DestroyElapsedTime += DeltaTime;
	if (DestroyElapsedTime >= DestroyDelay)
	{
		Destroy();
	}
}

void AHeavyProjecile::Launch()
{
	auto StaticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	if (StaticMeshComponent != nullptr)
	{
		StaticMeshComponent->AddImpulse(GetActorForwardVector() * LaunchForce, NAME_None, true);
	}
}

void AHeavyProjecile::OnHitDetected(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

