// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CharacterPawn.generated.h"

UCLASS()
class FIRSTUNREAL5PROJECT_API ACharacterPawn : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Movement")
		float _moveScale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player Movement")
		float _jumpScale;

	// Sets default values for this pawn's properties
	ACharacterPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MovePawnHorizontally(float);

	void MovePawnVertically(float);

	void JumpPawn();

};
