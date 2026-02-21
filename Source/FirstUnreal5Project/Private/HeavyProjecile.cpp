// Fill out your copyright notice in the Description page of Project Settings.


#include "HeavyProjecile.h"

// Sets default values
AHeavyProjecile::AHeavyProjecile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHeavyProjecile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHeavyProjecile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHeavyProjecile::Launch()
{
	UE_LOG(LogTemp, Warning, TEXT("Light Projectile Launched!"));
}

void AHeavyProjecile::OnHitDetected(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Light Projectile Hit Detected!"));
}

