// Fill out your copyright notice in the Description page of Project Settings.

#include <math.h>

#include "FirstUnreal5GameModeBase.h"

void AFirstUnreal5GameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	TimerComponent = NewObject<UTimerComponent>(this); // Create a new object of a particular type and assign it to something (who owns it?)
	check(TimerComponent != nullptr); // Assertion

	TimerComponent->RegisterComponent(); // Allow the componenet to update each frame

	TimerComponent->SetDuration(TimerDuration);

	TimerComponent->TimerFinished.AddDynamic(this, &AFirstUnreal5GameModeBase::OnTimerFinished);

	bMissionStarted = false;

	CurrentNumEnemies = 0;

	EnemiesDefeatedInWave = 0;

	WaveNumber = FMath::Min(NumWavesToClear, 0);
}

bool AFirstUnreal5GameModeBase::CanSpawnEnemies()
{
	return bMissionStarted && CurrentNumEnemies < MaxNumEnemies;
}

void AFirstUnreal5GameModeBase::AddEnemy()
{
	CurrentNumEnemies++;

	/*
	*	Eventually will have to use a pointer to the enemy character and use the class instance delegate to bind the OnEnemyDefeated function
	*/
}

void AFirstUnreal5GameModeBase::OnTimerFinished()
{
	bMissionStarted = false;

	/*
	*	Eventually will have to expand logic to handle what happens when the timer finishes during a mission, such as ending the mission and showing results
	*/
}

void AFirstUnreal5GameModeBase::OnBeginMission()
{
	bMissionStarted = true;

	TimerComponent->Start();
}

void AFirstUnreal5GameModeBase::OnEnemyDefeated()
{
	CurrentNumEnemies--;

	EnemiesDefeatedInWave++;

	if (EnemiesPerWave[WaveNumber] <= EnemiesDefeatedInWave)
	{
		WaveNumber++;

		EnemiesDefeatedInWave = 0;

		if (WaveNumber >= NumWavesToClear)
		{
			bMissionStarted = false;

			/*
			*	Eventually will have to expand logic to handle what happens when the timer finishes during a mission, such as ending the mission and showing results
			*/
		}
	}
}
