// Fill out your copyright notice in the Description page of Project Settings.


#include "LightProjectile.h"

// Sets default values
ALightProjectile::ALightProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UE_LOG(LogTemp, Warning, TEXT("[ALightProjectile::ALightProjectile]: Setting the destroy delay..."));

	DestroyDelay = 1.0f;
}

// Called when the game starts or when spawned
void ALightProjectile::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("[ALightProjectile::BeginPlay]: Light Projectile created..."));

	auto StaticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	if (StaticMeshComponent != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ALightProjectile::BeginPlay]: Mesh component found, setting up hit event..."));

		StaticMeshComponent->OnComponentHit.AddDynamic(this, &ALightProjectile::OnHitDetected);

		UE_LOG(LogTemp, Warning, TEXT("[ALightProjectile::BeginPlay]: Hit event setup successful!"));
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
		UE_LOG(LogTemp, Warning, TEXT("[ALightProjectile::Tick]: Destroying Light Projectile after delay..."));

		Destroy();
	}
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

void ALightProjectile::OnHitDetected(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("[ALightProjectile::OnHitDetected]: Light Projectile hit detected! Destroying the object..."));

	Destroy();

	UE_LOG(LogTemp, Warning, TEXT("[ALightProjectile::OnHitDetected]: Destroying Light Projectile after delay..."));
}
