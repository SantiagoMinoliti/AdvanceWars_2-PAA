// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameField.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerInterface.h"
#include "Gamefield.h"
#include "AW_GameMode.generated.h"

class ABaseCharacter;

UENUM()
enum class ECharacterId : uint32
{
	SANTA,
	BERNARD,
	GRINCH,
	MAX
};

UENUM()
enum class ECharacterType : uint32
{
	BRAWLER,
	SNIPER
};

/**
 * 
 */
UCLASS()
class ADVANCEWARS_2_API AAW_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	bool IsGameOver;

	TArray<IPlayerInterface*> Players;
	
	EPlayer CurrentPlayer;

	int32 CurrentPlayerNumber;

	int32 MoveCounter;
	
	TArray<ABaseCharacter*> AliveCharacters;
	
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGameField> GameFieldClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 FieldSize;

	UPROPERTY(VisibleAnywhere)
	AGameField* GField;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> SantaActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> BernardActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> GrinchActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> MaxActor;

	AAW_GameMode();

	virtual void BeginPlay() override;

	UFUNCTION()
	void ResetGameMode();
	
	void ChoosePlayerAndStartGame();
	
	EPlayer GetPlayerEncodeByPlayerNumber(int32 PlayerNumber);

	int32 GetPlayerNumberByPlayerEncode(EPlayer PlayerEncode);
	
	ECharacterId GetCharacterIdFBySubclass(TSubclassOf<AActor> ThisActor);

	TSubclassOf<AActor> GetSubclassByCharacterId(ECharacterId Id);

	ECharacterId GetCharacterIdByTileStatus(ETileStatus TileStatus);

	ETileStatus GetTileStatusByCharacterId(ECharacterId CharacterId);

	ABaseCharacter* GetCharacterByCharacterId(ECharacterId CharacterId);

	int32 GetNextPlayerNumber();

	void TurnNextPlayer();

	TArray<ABaseCharacter*> GetCurrentPlayerCharacters();
	
	TArray<ABaseCharacter*> GetCurrentPlayerAliveCharacters();
	
    void SpawnCharacter(ECharacterId CharacterId, ATile* SelectedTile);

	void MoveCharacter(ABaseCharacter* Char, ATile* SelectedTile);

	void InflictDamage(ATile* Tile, float Damage);
	
	void CheckWinConditions();
	
};
