// Fill out your copyright notice in the Description page of Project Settings.


#include "AW_PlayerController.h"

AAW_PlayerController::AAW_PlayerController() {
	bShowMouseCursor = true;
	bEnableClickEvents = true;
}

void AAW_PlayerController::ClickOnGrid() {
	const auto HumanPlayer = Cast<AHumanPlayer>(GetPawn());
	if(IsValid(HumanPlayer)) {
		HumanPlayer->OnClick();
	}
}

void AAW_PlayerController::BeginPlay() {
	Super::BeginPlay();
	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		Subsystem->AddMappingContext(AWContext, 0);
	}
}

void AAW_PlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &AAW_PlayerController::ClickOnGrid);
	}
}
