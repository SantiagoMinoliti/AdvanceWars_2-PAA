// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameField.h"
#include "AW_GameMode.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// AAW_GameMod* GameMode = Cast<AAW_GameMode>(GetWorld()->GetAuthGameMode());
	// GameMode->GField->OnResetEvent.AddDynamic(this, &ABaseCharacter::SelfDestroy);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SelfDestroy()
{
	Destroy();
}

