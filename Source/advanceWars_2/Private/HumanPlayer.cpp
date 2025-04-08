// Fill out your copyright notice in the Description page of Project Settings.


#include "HumanPlayer.h"
#include "GameField.h"
//#include "AW_GameMode.h"
#include "Components/InputComponent.h"
//#include "EnhancedInputComponent.h"
//#include "EnhancedInputSubsystems.h"

// Sets default values
AHumanPlayer::AHumanPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoRecieveInput::Player0;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SetRootComponent(Camera);

	//GameInstance = Cast<UGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	PlayerNumber = -1;
	Turn = ETeam::None;
}

// Called when the game starts or when spawned
void AHumanPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHumanPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHumanPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHumanPlayer::OnTurn() {
	isMyTurn = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Your Turn");
	//GameInstance->SetTurnMessage(TEXT("Human Turn"));
}

void AHumanPlayer::OnWin() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "You Win");
	//GameInstance->SetTurnMessage(TEXT("Human Wins"));
	//GameInstance->IncrementScoreHumanPlayer();
}

void AHumanPlayer::OnLose() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "You Lose");
	//GameInstance->SetTurnMessage(TEXT("CPU Wins"));
	//GameInstance->IncrementScoreCPUPlayer();

}

void AHumanPlayer::OnClick() {

	FHitResult Hit = FHitResult(ForceInit);

	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	if(Hit.bBlockingHit && isMyTurn) {
		if(ATile* CurrTile = Cast<ATile>(Hit.GetActor())) { // Logica di controllo, per ora placeholder: piazza tutti Santa
			if(CurrTile->SetTileStatus(PlayerNumber == ETileStatus::EMPTY) {
				CurrTile->SetTileStatus(PlayerNumber, ETileStatus::SANTA);
			}
		}
	}
}