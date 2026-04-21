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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destructible Properties")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destructible Properties")
	float InvincibilityDuration;

	FDestroyedDelegate DestroyedDelegate;

	// Sets default values for this actor's properties
	ADestructibleActor();

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
