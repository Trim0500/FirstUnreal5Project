// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDestroyedDelegate);

UCLASS()
class FIRSTUNREAL5PROJECT_API ADestructibleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	/** The HP of the actor ( in whole numbers ) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destructible Properties")
	float Health;

	/** The duration for which the actor is invincible after taking damage ( in sec ) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destructible Properties")
	float InvincibilityDuration;

	FDestroyedDelegate DestroyedDelegate;

	// Sets default values for this actor's properties
	ADestructibleActor();

	UFUNCTION()
	void OnHitDetected(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void TakeDamage(float DamageAmount);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Defeated();

private:
	bool bIsInvincible;

	FTimerHandle InvincibilityTimerHandle;

	UFUNCTION()
	void ResetInvincibility();
};
