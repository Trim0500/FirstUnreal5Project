// Fill out your copyright notice in the Description page of Project Settings.


#include "TimerComponent.h"

// Sets default values for this component's properties
UTimerComponent::UTimerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTimerComponent::BeginPlay()
{
	Super::BeginPlay();

	ElapsedSeconds = 0;

	TotalSeconds = 0;

	bRunning = false;

	bStarted = false;

	TickModifier = 1;
}


// Called every frame
void UTimerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bRunning)
	{
		ElapsedSeconds += DeltaTime * TickModifier;

		if (ElapsedSeconds >= TotalSeconds)
		{
			bRunning = false;

			TimerFinished.Broadcast();
		}
	}
}

void UTimerComponent::SetDuration(float Duration)
{
	if (!bRunning && Duration > 0)
	{
		TotalSeconds = Duration;
	}
}

void UTimerComponent::SetTickModifier(float Modifier)
{
	TickModifier = Modifier;
}

void UTimerComponent::Start()
{
	if (TotalSeconds > 0)
	{
		bStarted = true;

		bRunning = true;

		ElapsedSeconds = 0;
	}
}
