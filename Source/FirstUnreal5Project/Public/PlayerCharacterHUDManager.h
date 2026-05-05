// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerCharacterHUDManager.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTUNREAL5PROJECT_API APlayerCharacterHUDManager : public AHUD
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game HUD References")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
