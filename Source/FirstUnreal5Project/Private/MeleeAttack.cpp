// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAttack.h"

// Sets default values
AMeleeAttack::AMeleeAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMeleeAttack::BeginPlay()
{
	Super::BeginPlay();

	/*
	* [PC-05]: TODO
	*			Set the frame counter
	* 
	*			Set the flags for exceeding startup, active, and recovery frames to false
	* 
	*			Make actor invisible and non-collidable until startup frames are exceeded
	* 
	*			Get the reference to player controller and add a new input mapping context
	*/
}

// Called every frame
void AMeleeAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	* [PC-05]: TODO
	*			Increment frame counter by DeltaTime
	* 
	*			Establish logic gates for exceeding recovery, active, and startup frames.
	* 
	*			When Lifettime attack frames are exceeded, destroy the actor 
	* 
	*			When cancel recovery frames are exceeded, revoke mapping context for this attack from player controller and set exceeded cancel recovery frames flag to true to prevent this logic gate from re-entered
	*				if another attack may come after this one in a defined sequence, get reference to player controller and replace mapping context for this attack with mapping context for next attack in sequence
	*				if another attack may not come after this one in a defined sequence, destroy the actor
	* 
	*			When recovery frames are exceeded, set mesh to be invisible and non-collidable and set exceeded recovery frames flag to true to prevent this logic gate from being re-entered
	* 
	*			When active frames are exceeded, set the actor to be visible and collidable and set exceeded active frames flag to true to prevent this logic gate from being re-entered
	*/
}
