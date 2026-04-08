// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstUnreal5GameModeBase.h"

void AFirstUnreal5GameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	TimerComponent = NewObject<UTimerComponent>(this); // Create a new object of a particular type and assign it to something (who owns it?)
	check(TimerComponent != nullptr); // Assertion

	TimerComponent->RegisterComponent(); // Allow the componenet to update each frame

	TimerComponent->SetDuration(TimerDuration);

	TimerComponent->Start();

	TimerComponent->TimerFinished.AddDynamic(this, &AFirstUnreal5GameModeBase::OnTimerFinished);
}

void AFirstUnreal5GameModeBase::OnTimerFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("[AFirstUnreal5GameModeBase::OnTimerFinished]: Function called..."));
}
