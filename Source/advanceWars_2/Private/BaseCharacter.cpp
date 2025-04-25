// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameField.h"
#include "AW_GameMode.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	CharacterId = ECharacterId::SANTA;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterId = CharacterId;

	if (CharacterId == ECharacterId::BERNARD || CharacterId == ECharacterId::MAX)
	{
		CharacterType = ECharacterType::SNIPER;
		MaxHealth = 20.f;
		Health = MaxHealth;
		TileSpeed = 3;
		TileRange = 10;
		MinDamage = 4.f;
		MaxDamage = 8.f;
	} else if (CharacterId == ECharacterId::SANTA || CharacterId == ECharacterId::GRINCH)
	{
		CharacterType = ECharacterType::BRAWLER;
		MaxHealth = 40.f;
		Health = MaxHealth;
		TileSpeed = 6;
		TileRange = 1;
		MinDamage = 1.f;
		MaxDamage = 6.f;
	}

	bIsHumanPlayerCharacter = CharacterId == ECharacterId::SANTA || CharacterId == ECharacterId::BERNARD;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	GameMode = (AAW_GameMode*)(GetWorld()->GetAuthGameMode());
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

ECharacterId ABaseCharacter::GetCharacterId()
{
	return CharacterId;
}

ETileStatus ABaseCharacter::GetCorrespondingTileStatus()
{
	switch (CharacterId)
	{
	case ECharacterId::SANTA:
		return ETileStatus::SANTA;
	case ECharacterId::BERNARD:
		return ETileStatus::BERNARD;
	case ECharacterId::GRINCH:
		return ETileStatus::GRINCH;
	case ECharacterId::MAX:
		return ETileStatus::MAX;
	default:
		return ETileStatus::EMPTY;
	}
}

bool ABaseCharacter::IsHumanPlayerCharacter()
{
	return bIsHumanPlayerCharacter;
}

bool ABaseCharacter::IsAlive()
{
	return Health > 0;
}

ATile* ABaseCharacter::GetStandingTile() const
{
	return StandingTile;
}

void ABaseCharacter::SetStandingTile(ATile* Tile)
{
	StandingTile = Tile;
}

void ABaseCharacter::Move(ATile* SelectedTile)
{
	GameMode->MoveCharacter(this, SelectedTile);
	FVector2D TargetLocation = SelectedTile->GetPosition();
	SetActorLocation(FVector(TargetLocation.X, TargetLocation.Y, 0));
}

