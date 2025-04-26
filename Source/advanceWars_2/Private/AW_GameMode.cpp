// Fill out your copyright notice in the Description page of Project Settings.


#include "AW_GameMode.h"
#include "AW_PlayerController.h"
#include "BaseCharacter.h"
#include "HumanPlayer.h"
#include "RandomPlayer.h"
#include "MinimaxPlayer.h"
#include "EngineUtils.h"

AAW_GameMode::AAW_GameMode() {
	PlayerControllerClass = AAW_PlayerController::StaticClass();
	DefaultPawnClass = AHumanPlayer::StaticClass();
	FieldSize = 25;
}

void AAW_GameMode::BeginPlay() {
	Super::BeginPlay();
	IsGameOver = false;
	MoveCounter = 0;
	AHumanPlayer* HumanPlayer = Cast<AHumanPlayer>(*TActorIterator<AHumanPlayer>(GetWorld()));
	if(GameFieldClass != nullptr) {
		GField = GetWorld()->SpawnActor<AGameField>(GameFieldClass);
		GField->Size = FieldSize;
		GField->GenerateField();
	} else {
		UE_LOG(LogTemp, Error, TEXT("Game Field is null"));
	}
	float CameraPosX = ((GField->TileSize * GField->Size / 2));
	FVector CameraPos(CameraPosX, CameraPosX, 1000.0f);
	HumanPlayer->SetActorLocationAndRotation(CameraPos, FRotationMatrix::MakeFromX(FVector(0,0,-1)).Rotator());

	Players.Add(HumanPlayer);

	auto* CPUPlayer = GetWorld()->SpawnActor<ARandomPlayer>(FVector(), FRotator());
	Players.Add(CPUPlayer);
	
}

void AAW_GameMode::ChoosePlayerAndStartGame()
{
	CurrentPlayerNumber = FMath::RandRange(0, Players.Num() - 1);

	for (int32 i = 0; i < Players.Num(); i++)
	{
		Players[i]->PlayerNumber = i;
	}
	MoveCounter++;
	Players[CurrentPlayerNumber]->OnSetupTurn();
}

EPlayer AAW_GameMode::GetPlayerEncodeByPlayerNumber(int32 PlayerNumber)
{
	return Players[PlayerNumber]->PlayerEncode;
}

int32 AAW_GameMode::GetPlayerNumberByPlayerEncode(EPlayer PlayerEncode)
{
	for (auto* Player : Players)
	{
		if (Player->PlayerEncode == PlayerEncode) return Player->PlayerNumber;
	}
	return NULL;
}

ECharacterId AAW_GameMode::GetCharacterIdFBySubclass(TSubclassOf<AActor> ThisActor)
{
	if (ThisActor == SantaActor)
	{
		return ECharacterId::SANTA;
	}
	else if (ThisActor == BernardActor) 
	{
		return ECharacterId::BERNARD;
	}
	else if (ThisActor == GrinchActor) 
	{
		return ECharacterId::GRINCH;
	}
	else if (ThisActor == MaxActor)
	{
		return ECharacterId::MAX;
	}
	else return ECharacterId::SANTA;
}

TSubclassOf<AActor> AAW_GameMode::GetSubclassByCharacterId(ECharacterId Id)
{
	switch (Id)
	{
	case ECharacterId::SANTA:
		return SantaActor;
	case ECharacterId::BERNARD:
		return BernardActor;
	case ECharacterId::GRINCH:
		return GrinchActor;
	case ECharacterId::MAX:
		return MaxActor;
	default:
		return SantaActor;
	}
}

int32 AAW_GameMode::GetNextPlayerNumber()
{
	return 1 - CurrentPlayerNumber;
}

void AAW_GameMode::TurnNextPlayer()
{
	if (IsGameOver) return;
	MoveCounter++;
	CurrentPlayerNumber = GetNextPlayerNumber();
	auto* Player = Players[CurrentPlayerNumber];
	CurrentPlayer = Player->PlayerEncode;
	Player->OnTurn();
}

TArray<ABaseCharacter*> AAW_GameMode::GetCurrentPlayerCharacters()
{
	TArray<ABaseCharacter*> Chars;
	for (ABaseCharacter* Char : AliveCharacters)
	{
		if (Char->IsHumanPlayerCharacter() && CurrentPlayer == EPlayer::HUMAN || !Char->IsHumanPlayerCharacter() && CurrentPlayer == EPlayer::CPU)
		{
			Chars.Add(Char);
		}
	}
	return Chars;
}

ECharacterId AAW_GameMode::GetCharacterIdByTileStatus(ETileStatus TileStatus)
{
	switch (TileStatus)
	{
	case ETileStatus::SANTA:
		return ECharacterId::SANTA;
	case ETileStatus::BERNARD:
		return ECharacterId::BERNARD;
	case ETileStatus::GRINCH:
		return ECharacterId::GRINCH;
	case ETileStatus::MAX:
		return ECharacterId::MAX;
	default:
		return ECharacterId::SANTA;
	}
}

ETileStatus AAW_GameMode::GetTileStatusByCharacterId(ECharacterId CharacterId)
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
		return ETileStatus::SANTA;
	}
}

ABaseCharacter* AAW_GameMode::GetCharacterByCharacterId(ECharacterId CharacterId)
{
	for (ABaseCharacter* Character : AliveCharacters) if (Character->GetCharacterId() == CharacterId) return Character;
	return nullptr;
}

TArray<ABaseCharacter*> AAW_GameMode::GetCurrentPlayerAliveCharacters()
{
	TArray<ABaseCharacter*> Chars = GetCurrentPlayerCharacters();
	TArray<ABaseCharacter*> AliveChars;
	for (ABaseCharacter* Char : Chars) {
		if (Char->IsAlive()) {
			AliveChars.Add(Char);
		}
	}
	return AliveChars;
}

void AAW_GameMode::SpawnCharacter(ECharacterId CharacterId, ATile* SelectedTile)
{
	FVector2D Location = SelectedTile->GetPosition();
	ABaseCharacter* Char = GetWorld()->SpawnActor<ABaseCharacter>(GetSubclassByCharacterId(CharacterId), FVector(Location.X, Location.Y, 0), FRotator::ZeroRotator);
	SelectedTile->SetStatus(Char->GetCorrespondingTileStatus());
	Char->SetStandingTile(SelectedTile);
	AliveCharacters.Add(Char);
}

void AAW_GameMode::MoveCharacter(ABaseCharacter* Char, ATile* SelectedTile)
{
	Char->GetStandingTile()->SetStatus(ETileStatus::EMPTY);
	SelectedTile->SetStatus(Char->GetCorrespondingTileStatus());
	Char->SetStandingTile(SelectedTile);
}

void AAW_GameMode::InflictDamage(ATile* Tile, float Damage)
{
	ETileStatus TileStatus = Tile->GetStatus();
	ABaseCharacter* Char = GetCharacterByCharacterId(GetCharacterIdByTileStatus(TileStatus));
	float CurrentHealth = Char->GetHealth();
	float NewHealth = CurrentHealth + Damage;
	if(NewHealth < 0) NewHealth = 0;
	Char->SetHealth(NewHealth);
	if (NewHealth < 0) {
		AliveCharacters.Remove(Char);
		Char->SelfDestroy();
	}
	CheckWinConditions();
}

void AAW_GameMode::CheckWinConditions()
{
	bool bSantaTeamAlive, bGrinchTeamAlive = false;
	for (ABaseCharacter* Char : AliveCharacters)
	{
		if (Char->CharacterId == ECharacterId::SANTA || Char->CharacterId == ECharacterId::BERNARD) bSantaTeamAlive = true;
		else if (Char->CharacterId == ECharacterId::GRINCH || Char->CharacterId == ECharacterId::MAX) bGrinchTeamAlive = true;
	}
	if (bSantaTeamAlive && !bGrinchTeamAlive)
	{
		IsGameOver = true;
		for(auto* Player : Players)
		{
			if (Player->PlayerEncode == EPlayer::HUMAN) Player->OnWin();
			else Player->OnLose();
		}
	}
	else if (!bSantaTeamAlive && bGrinchTeamAlive)
	{
		IsGameOver = true;
		for(auto* Player : Players)
		{
			if (Player->PlayerEncode == EPlayer::CPU) Player->OnWin();
			else Player->OnLose();
		}
	}
}
