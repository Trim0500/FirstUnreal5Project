// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleActor.h"

// Sets default values
ADestructibleActor::ADestructibleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADestructibleActor::BeginPlay()
{
	Super::BeginPlay();

	auto StaticMeshComponent = FindComponentByClass<UStaticMeshComponent>();
	if (StaticMeshComponent != nullptr)
	{
		StaticMeshComponent->OnComponentHit.AddDynamic(this, &ADestructibleActor::OnHitDetected);
	}
}

// Called every frame
void ADestructibleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADestructibleActor::OnHitDetected(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	TakeDamage(10.0f);
}

void ADestructibleActor::TakeDamage(float DamageAmount)
{
	if (bIsInvincible)
	{
		return;
	}

	if (!bIsInvincible)
	{
		bIsInvincible = true;
	}

	Health -= DamageAmount;
	if (Health <= 0.0f)
	{
		Defeated();

		return;
	}

	GetWorld()->GetTimerManager().SetTimer(InvincibilityTimerHandle, this, &ADestructibleActor::ResetInvincibility, InvincibilityDuration, false);
}

void ADestructibleActor::Defeated()
{
	DestroyedDelegate.Broadcast();

	Destroy();
}

void ADestructibleActor::ResetInvincibility()
{
	bIsInvincible = false;
}

