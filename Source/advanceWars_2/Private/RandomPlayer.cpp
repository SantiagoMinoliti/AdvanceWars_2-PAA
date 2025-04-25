// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomPlayer.h"

#include "BaseCharacter.h"

// Sets default values
ARandomPlayer::ARandomPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GameInstance = Cast<UAW_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

}

// Called when the game starts or when spawned
void ARandomPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARandomPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARandomPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

TArray<ABaseCharacter*> ShuffleCharacters(TArray<ABaseCharacter*> Characters)
{
	TArray<ABaseCharacter*> ShuffledCharacters = Characters;

	for (int32 i = ShuffledCharacters.Num() - 1; i > 0; i--)
	{
		int32 j = FMath::RandRange(0, i);
		if (i != j)
		{
			ShuffledCharacters.Swap(i, j);
		}
	}

	return ShuffledCharacters;
}

void ARandomPlayer::OnSetupTurn()
{
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,"Turn");
	//GameInstance->SetTurnMessage(TEXT("CPU (Random) Turn");
	AAW_GameMode* GameMode = (AAW_GameMode*)(GetWorld()->GetAuthGameMode());

	TArray<ABaseCharacter*> Characters = GameMode->GetCurrentPlayerAliveCharacters();
	ECharacterId CharacterId;
	if (Characters.Num() > 0) {
		ECharacterId ExistingCharId = Characters[0]->GetCharacterId();
		if (ExistingCharId == ECharacterId::GRINCH) CharacterId = ECharacterId::MAX;
		else if (ExistingCharId == ECharacterId::MAX) CharacterId = ECharacterId::GRINCH;
	} else {
		if (FMath::Rand() % 2 == 0) CharacterId = ECharacterId::MAX;
		else CharacterId = ECharacterId::GRINCH;
	}

	ATile* ExtractedTile = GameMode->GField->GetRandomEmptyTile();

	GameMode->SpawnCharacter(CharacterId, ExtractedTile);
}

void ARandomPlayer::OnTurn() {
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,"Turn");
	//GameInstance->SetTurnMessage(TEXT("CPU (Random) Turn");
	AAW_GameMode* GameMode = (AAW_GameMode*)(GetWorld()->GetAuthGameMode());
	TArray<ABaseCharacter*> Characters = ShuffleCharacters(GameMode->GetCurrentPlayerAliveCharacters());

	FTimerHandle TimerHandle;
	for (auto& CurrCharacter : Characters) {
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			TArray<ATile*> Tiles = CurrCharacter->GetReachableTiles();
			if (Tiles.Num() > 0)
			{
				int32 RandIdx = FMath::Rand() % Tiles.Num();
				ATile* SelectedTile = Tiles[RandIdx];

				CurrCharacter->Move(SelectedTile);
			}
			TArray<ATile*> AttackOptions = CurrCharacter->GetAttackOptions();
			if (FMath::Rand() % 4 != 0 && AttackOptions.Num() > 0)
			{
				CurrCharacter->Attack(AttackOptions[FMath::Rand() % AttackOptions.Num()]);
			}
			
		}, 3.f, false);
	}
	
	
}

void ARandomPlayer::OnWin() {
	
}

void ARandomPlayer::OnLose() {
	
}
