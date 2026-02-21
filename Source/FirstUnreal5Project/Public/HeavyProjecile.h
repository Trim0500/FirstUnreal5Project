// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Projectile.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeavyProjecile.generated.h"

UCLASS()
class FIRSTUNREAL5PROJECT_API AHeavyProjecile : public AActor, public IProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHeavyProjecile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Launch() override;

	UFUNCTION()
	virtual void OnHitDetected(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	/** Force to apply onto a projectile when it is fired after being spawned in */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Projectile Parameters")
	float LaunchForce;

	/** Projectile damage value when it hits a destructible object or enemy */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Projectile Parameters")
	int Power;

};
