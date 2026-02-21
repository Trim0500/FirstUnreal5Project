// Fill out your copyright notice in the Description page of Project Settings.


#include "HeavyProjecile.h"

// Sets default values
AHeavyProjecile::AHeavyProjecile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UE_LOG(LogTemp, Warning, TEXT("[AHeavyProjecile::AHeavyProjecile]: Setting the destroy delay..."));

	DestroyDelay = 1.0f;
}

// Called when the game starts or when spawned
void AHeavyProjecile::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("[AHeavyProjecile::BeginPlay]: Heavy Projectile created..."));

	auto StaticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	if (StaticMeshComponent != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AHeavyProjecile::BeginPlay]: Mesh component found, setting up hit event..."));

		StaticMeshComponent->OnComponentHit.AddDynamic(this, &AHeavyProjecile::OnHitDetected);

		UE_LOG(LogTemp, Warning, TEXT("[AHeavyProjecile::BeginPlay]: Hit event setup successful!"));
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
		UE_LOG(LogTemp, Warning, TEXT("[AHeavyProjecile::Tick]: Destroying Heavy Projectile after delay..."));

		Destroy();
	}
}

void AHeavyProjecile::Launch()
{
	UE_LOG(LogTemp, Warning, TEXT("[AHeavyProjecile::Launch]: Heavy Projectile will be launched..."));

	auto StaticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	if (StaticMeshComponent != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AHeavyProjecile::Launch]: Mesh component found, applying force..."));

		StaticMeshComponent->AddImpulse(GetActorForwardVector() * LaunchForce, NAME_None, true);

		UE_LOG(LogTemp, Warning, TEXT("[AHeavyProjecile::Launch]: Launch successful!"));
	}
}

void AHeavyProjecile::OnHitDetected(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("[AHeavyProjecile::OnHitDetected]: Light Projectile hit detected! Destroying the object..."));

	Destroy();

	UE_LOG(LogTemp, Warning, TEXT("[AHeavyProjecile::OnHitDetected]: Destroying Light Projectile after delay..."));
}

