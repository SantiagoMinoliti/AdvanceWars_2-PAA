// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HumanPlayer.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AW_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEWARS_2_API AAW_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAW_PlayerController();
	
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* AWContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ClickAction;

	void ClickOnGrid();
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
};
