// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterHUDManager.h"
#include "Blueprint/UserWidget.h"

void APlayerCharacterHUDManager::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("[APlayerCharacterHUDManager::BeginPlay] Function called..."));
	
	Super::BeginPlay();

	if (PlayerHUDWidgetClass)
	{
		UUserWidget* PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
		if (PlayerHUDWidget)
		{
			PlayerHUDWidget->AddToViewport();
		}
	}
}
