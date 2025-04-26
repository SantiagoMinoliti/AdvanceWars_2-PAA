// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameField.h"
#include "AW_GameMode.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	CharacterId = ECharacterId::SANTA;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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

	GameMode = Cast<AAW_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->GField->OnResetEvent.AddDynamic(this, &ABaseCharacter::SelfDestroy);
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

float ABaseCharacter::GetHealth()
{
	return Health;
}

void ABaseCharacter::SetHealth(float NewHealth)
{
	Health = NewHealth;
}

ATile* ABaseCharacter::GetStandingTile() const
{
	return StandingTile;
}

void ABaseCharacter::SetStandingTile(ATile* Tile)
{
	StandingTile = Tile;
}

TArray<ATile*> ABaseCharacter::GetReachableTiles()
{
	return GameMode->GField->GetReachableTiles(StandingTile, TileSpeed, false);
}

void ABaseCharacter::Move(ATile* SelectedTile)
{
	GameMode->MoveCharacter(this, SelectedTile);
	FVector2D TargetLocation = SelectedTile->GetPosition();
	SetActorLocation(FVector(TargetLocation.X, TargetLocation.Y, 0));
}


TArray<ATile*> ABaseCharacter::GetAttackOptions()
{
	TArray<ATile*> ReachableTiles = GameMode->GField->GetReachableTiles(StandingTile, TileRange, CharacterType == ECharacterType::SNIPER);
	ETileStatus OpponentBrawler, OpponentSniper;
	ETileStatus CorrespondingTileStatus = GetCorrespondingTileStatus();
	if (CorrespondingTileStatus == ETileStatus::SANTA || CorrespondingTileStatus == ETileStatus::BERNARD)
	{
		OpponentBrawler = ETileStatus::GRINCH;
		OpponentSniper = ETileStatus::MAX;
	} else if (CorrespondingTileStatus == ETileStatus::GRINCH || CorrespondingTileStatus == ETileStatus::MAX)
	{
		OpponentBrawler = ETileStatus::SANTA;
		OpponentSniper = ETileStatus::BERNARD;
	}
	TArray<ATile*> AttackOptions;
	for (ATile* ReachableTile : ReachableTiles)
	{
		ETileStatus ReachableTileStatus = ReachableTile->GetStatus();
		if (ReachableTileStatus == OpponentBrawler || ReachableTileStatus == OpponentSniper)
		{
			AttackOptions.Add(ReachableTile);
		}
	}
	return AttackOptions;
}

void ABaseCharacter::Attack(ATile* Tile)
{
	float Damage = CalculateDamage();
	GameMode->InflictDamage(Tile, Damage);
}

float ABaseCharacter::CalculateDamage()
{
	float Damage = (FMath::Rand() * (MaxDamage - MinDamage) + MinDamage) * (-1);
	return Damage;
}


