#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "AW_HUD.generated.h"

enum class ECharacterId : uint32;

/**
 * 
 */
UCLASS()
class ADVANCEWARS_2_API UAW_HUD : public UUserWidget
{
	GENERATED_BODY()

public:
	// Variabili per i TextBlock
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SantaHealthTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BernardHealthTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GrinchHealthTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxHealthTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TurnTextBlock;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterTypeTextBlock; // Testo per il tipo di personaggio

	// Variabile per il turno (player o CPU)
	UPROPERTY()
	bool bIsPlayerTurn = true;

	// Funzione per settare il testo di TurnTextBlock
	UFUNCTION(BlueprintCallable)
	void SetTurnText();

	// Funzione per togglare il turno
	UFUNCTION(BlueprintCallable)
	void ToggleTurn();

	// Funzione per togglare il tipo di personaggio
	UFUNCTION(BlueprintCallable)
	void ToggleCharacterType();

	// Funzione per settare la salute
	UFUNCTION(BlueprintCallable)
	void SetHealthText(ECharacterId CharacterId, int32 Health);

	// Funzione per gestire la visibilità dei TextBlock di salute
	UFUNCTION(BlueprintCallable)
	void SetHealthVisibility(bool bIsVisible);

	// Funzioni per la visibilità di altri componenti (testo turno, bottone, etc.)
	UFUNCTION(BlueprintCallable)
	void SetTurnTextVisibility(bool bIsVisible);

	UFUNCTION(BlueprintCallable)
	void SetCharacterTypeVisibility(bool bIsVisible);

	// Variabile per il tipo di personaggio (Brawler o Sniper)
	UPROPERTY()
	FString CharacterType = "Brawler";
protected:
	// Funzione per inizializzare il bottone
	virtual void NativeConstruct() override;

private:


	// Bottone per togglare il tipo di personaggio
	UPROPERTY(meta = (BindWidget))
	UButton* ToggleCharacterButton;

	// Funzione chiamata quando il bottone viene cliccato
	UFUNCTION()
	void OnToggleCharacterButtonClicked();
};
