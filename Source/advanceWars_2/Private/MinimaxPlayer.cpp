// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimaxPlayer.h"

// Sets default values
AMinimaxPlayer::AMinimaxPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMinimaxPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMinimaxPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMinimaxPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

