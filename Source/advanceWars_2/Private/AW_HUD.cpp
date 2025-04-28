#include "AW_HUD.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

// Funzione che setta il testo del TurnTextBlock in base al turno corrente
void UAW_HUD::SetTurnText()
{
	if (TurnTextBlock)
	{
		if (bIsPlayerTurn)
		{
			TurnTextBlock->SetText(FText::FromString("Your Turn"));
		}
		else
		{
			TurnTextBlock->SetText(FText::FromString("CPU Turn"));
		}
	}
}

// Funzione che toggla il turno
void UAW_HUD::ToggleTurn()
{
	bIsPlayerTurn = !bIsPlayerTurn; // Cambia il turno
	SetTurnText(); // Aggiorna il testo del TurnTextBlock
}

// Funzione che toggla il tipo di personaggio
void UAW_HUD::ToggleCharacterType()
{
	if (CharacterType == "Brawler")
	{
		CharacterType = "Sniper"; // Cambia il tipo di personaggio
	}
	else
	{
		CharacterType = "Brawler"; // Cambia di nuovo a Brawler
	}

	// Aggiorna il testo
	if (CharacterTypeTextBlock)
	{
		CharacterTypeTextBlock->SetText(FText::FromString(CharacterType));
	}
}

// Funzione che viene chiamata quando il widget viene costruito
void UAW_HUD::NativeConstruct()
{
	Super::NativeConstruct();

	// Setta il testo iniziale per il TurnTextBlock
	SetTurnText();

	// Collega la funzione al bottone
	if (ToggleCharacterButton)
	{
		ToggleCharacterButton->OnClicked.AddDynamic(this, &UAW_HUD::OnToggleCharacterButtonClicked);
	}
}

// Funzione chiamata quando il bottone viene cliccato
void UAW_HUD::OnToggleCharacterButtonClicked()
{
	ToggleCharacterType(); // Cambia il tipo di personaggio
}

// Funzione per settare la salute dei personaggi
void UAW_HUD::SetHealthText(ECharacterId CharacterId, int32 Health)
{
	UTextBlock* HealthTextBlock = nullptr;

	// Seleziona il TextBlock giusto in base al CharacterId
	switch (CharacterId)
	{
	case ECharacterId::Santa:
		HealthTextBlock = SantaHealthTextBlock;
		break;
	case ECharacterId::Bernard:
		HealthTextBlock = BernardHealthTextBlock;
		break;
	case ECharacterId::Grinch:
		HealthTextBlock = GrinchHealthTextBlock;
		break;
	case ECharacterId::Max:
		HealthTextBlock = MaxHealthTextBlock;
		break;
	default:
		return; // Non fare nulla se l'ID non è valido
	}

	// Imposta il testo del TextBlock
	if (HealthTextBlock)
	{
		HealthTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), Health)));
	}
}

// Funzione per gestire la visibilità dei TextBlock di salute
void UAW_HUD::SetHealthVisibility(bool bIsVisible)
{
	SantaHealthTextBlock->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	BernardHealthTextBlock->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	GrinchHealthTextBlock->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	MaxHealthTextBlock->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

// Funzione per gestire la visibilità del TurnTextBlock
void UAW_HUD::SetTurnTextVisibility(bool bIsVisible)
{
	if (TurnTextBlock)
	{
		TurnTextBlock->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

// Funzione per gestire la visibilità del CharacterTypeTextBlock
void UAW_HUD::SetCharacterTypeVisibility(bool bIsVisible)
{
	if (CharacterTypeTextBlock)
	{
		CharacterTypeTextBlock->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

// Funzione per gestire la visibilità del bottone Toggle
void UAW_HUD::SetToggleButtonVisibility(bool bIsVisible)
{
	if (ToggleCharacterButton)
	{
		ToggleCharacterButton->SetVisibility(bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}
