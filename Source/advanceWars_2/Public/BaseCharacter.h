// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AW_GameMode.h"
#include "GameFramework/Actor.h"
#include "Tile.h"
#include "BaseCharacter.generated.h"


UCLASS()
class ADVANCEWARS_2_API ABaseCharacter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character")
	bool bIsHumanPlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character")
	ECharacterId CharacterId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character")
	ECharacterType CharacterType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character")
	int32 TileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character")
	float MinDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character")
	float MaxDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character")
	int32 TileRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	AAW_GameMode* GameMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	ATile* StandingTile;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void SelfDestroy();

	ECharacterId GetCharacterId();

	ETileStatus GetCorrespondingTileStatus();

	bool IsHumanPlayerCharacter();

	bool IsAlive();


	ATile* GetStandingTile() const;

	void SetStandingTile(ATile* Tile);
	
	TArray<ATile*> GetReachableTiles();
	
	void Move(ATile* Tile);

	TArray<ATile*> GetAttackOptions();

	void Attack(ATile* Tile);

	// TODO: tutte le funzioni
};
