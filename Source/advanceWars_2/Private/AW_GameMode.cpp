// Fill out your copyright notice in the Description page of Project Settings.

#include "AW_GameMode.h"
#include "AW_PlayerController.h"
#include "BaseCharacter.h"
#include "HumanPlayer.h"
#include "GameFramework/HUD.h"
#include "AW_HUD.h"
#include "RandomPlayer.h"
#include "MinimaxPlayer.h"
#include "EngineUtils.h"

AAW_GameMode::AAW_GameMode() {
	PlayerControllerClass = AAW_PlayerController::StaticClass();
	DefaultPawnClass = AHumanPlayer::StaticClass();
	FieldSize = 25;

	static ConstructorHelpers::FClassFinder<UUserWidget> WBP(TEXT("/Game/Blueprints/BP_HUD.BP_HUD_C"));
    if (WBP.Succeeded())
    {
        HUD1Class = WBP.Class;
        UE_LOG(LogTemp, Warning, TEXT("Blueprint trovato!"));
    }
}

void AAW_GameMode::BeginPlay() {
	Super::BeginPlay();

	HUD = Cast<UAW_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	// Assicurati che l'HUD sia correttamente inizializzato

	if (HUD)
	{
		// Nascondi le vite dei personaggi
		HUD->SetHealthVisibility(false);

		// Rendi visibili il bottone e il testo "Brawler"
		HUD->SetCharacterTypeVisibility(false);

		HUD->CharacterType = "Choose Character";  // Imposta direttamente il tipo di personaggio
		if (HUD->CharacterTypeTextBlock)
		{
			HUD->CharacterTypeTextBlock->SetText(FText::FromString(HUD->CharacterType));
			HUD->CharacterType = "Choose Character";
		}
	}
	
	IsGameOver = false;
	MoveCounter = 0;
	if(GameFieldClass != nullptr) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GField = GetWorld()->SpawnActor<AGameField>(GameFieldClass, SpawnParams);
		UE_LOG(LogTemp, Warning, TEXT("Game Field: %s"), *GetNameSafe(GField));
		GField->Size = FieldSize;
		GField->GenerateField();
		GField->OnResetEvent.AddDynamic(this, &AAW_GameMode::ResetGameMode);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Game Field Class is null, GameFieldClass: %s"), *GetNameSafe(GameFieldClass));
	}

if (HUD1Class)
{
    HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUD1Class);
    if (HUDWidget)
    {
       HUDWidget->AddToViewport();
       UE_LOG(LogTemp, Warning, TEXT("HUD aggiunta alla viewport"));
    }
    else
    {
       UE_LOG(LogTemp, Error, TEXT(" Errore: impossibile creare HUDWidget!"));
    }
}
	
}

void AAW_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Set up a delayed call to wait until the Pawn is spawned
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, [this, NewPlayer]()
	{
		if (!IsValid(NewPlayer))
		{
			UE_LOG(LogTemp, Error, TEXT("NewPlayer invalid in delayed PostLogin!"));
			return;
		}

		AHumanPlayer* HumanPlayer = Cast<AHumanPlayer>(NewPlayer->GetPawn());
		if (HumanPlayer)
		{
			Players.Add(HumanPlayer);
			UE_LOG(LogTemp, Error, TEXT("Found HumanPlayer after delay: %s"), *GetNameSafe(HumanPlayer));

			// Imposta la posizione della telecamera (già esistente nel tuo codice)
			float CameraPosX = ((GField->TileSize * GField->Size / 2));
			FVector CameraPos(CameraPosX, CameraPosX, 3370.f);

			// Imposta la telecamera per guardare dall'alto, lungo l'asse Z negativo
			FRotator CameraRotation = FRotator(-90.f, 0.f, 0.f); // Guardare direttamente verso il basso

			// Imposta la telecamera come ortografica
			UCameraComponent* CameraComponent = HumanPlayer->FindComponentByClass<UCameraComponent>();
			if (CameraComponent)
			{
				CameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);  // Imposta la proiezione ortografica
				CameraComponent->SetOrthoWidth(2048.f); // Imposta la larghezza ortografica in base alla tua scena
			}

			// Imposta la posizione e rotazione dell'attore (la telecamera)
			HumanPlayer->SetActorLocationAndRotation(CameraPos, CameraRotation);

			auto* CPUPlayer = GetWorld()->SpawnActor<ARandomPlayer>(FVector(), FRotator());
			Players.Add(CPUPlayer);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("HumanPlayer still not found after delay!"));
		}

	}, 0.1f, false); // delay 0.1 secondi
}




void AAW_GameMode::ResetGameMode()
{
	IsGameOver = false;
	MoveCounter = 0;

	Players.Empty();
	
	ChoosePlayerAndStartGame();
}

void AAW_GameMode::ChoosePlayerAndStartGame()
{
	CurrentPlayerNumber = FMath::RandRange(0, Players.Num() - 1);

	for (int32 i = 0; i < Players.Num(); i++)
	{
		Players[i]->PlayerNumber = i;
	}
	MoveCounter++;
	HUD->bIsPlayerTurn = Players[CurrentPlayerNumber]->PlayerEncode == EPlayer::HUMAN;
	HUD->SetTurnText();
	if (HUD->bIsPlayerTurn) HUD->SetCharacterTypeVisibility(true);
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
	HUD->ToggleTurn();
	Player->OnTurn();
}

// CONTINUE HERE: HumanPlayer OnTurn and OnSetupTurn (if there are 2 characters left to place look in the hud what is CharacterType if "Brawler" or "Sniper" or else and permit to place it if valid

void AAW_GameMode::SetupTurnNextPlayer()
{
	if (AliveCharacters.Num() == 4)
	{
		for(auto* AliveCharacter : AliveCharacters) HUD->SetHealthText(AliveCharacter->GetCharacterId(), AliveCharacter->GetHealth());
		HUD->SetHealthVisibility(true);
		HUD->SetCharacterTypeVisibility(false);
		TurnNextPlayer();
	} else {
		CurrentPlayerNumber = GetNextPlayerNumber();
		auto* Player = Players[CurrentPlayerNumber];
		CurrentPlayer = Player->PlayerEncode;
		HUD->SetCharacterTypeVisibility(AliveCharacters.Num() == 1 && Player->PlayerEncode == EPlayer::HUMAN);
		Player->OnSetupTurn();
	}
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
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector2D Location = SelectedTile->GetPosition();
	ABaseCharacter* Char = GetWorld()->SpawnActor<ABaseCharacter>(GetSubclassByCharacterId(CharacterId), FVector(Location.X, Location.Y, 0), FRotator::ZeroRotator, SpawnParams);
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
	HUD->SetHealthText(GetCharacterIdByTileStatus(TileStatus), NewHealth);
	CheckWinConditions();
}

void AAW_GameMode::CheckWinConditions()
{
	bool bSantaTeamAlive = false;
	bool bGrinchTeamAlive = false;
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
