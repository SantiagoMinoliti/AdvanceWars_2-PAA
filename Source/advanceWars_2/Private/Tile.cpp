#include "Tile.h"

#include "AW_GameMode.h"
#include "GameModeInfoCustomizer.h"

// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	SetRootComponent(Scene);
	StaticMeshComponent->SetupAttachment(Scene);

	Status = ETileStatus::EMPTY;
	TileGridPosition = FIntPoint(0, 0);
}

void ATile::SetStatus(const ETileStatus _status) {
	Status = _status;
}

ETileStatus ATile::GetStatus() const {
	return Status;
}

void ATile::SetPosition(const double InX, const double InY) {
	TileGridPosition.X = InX;
	TileGridPosition.Y = InY;
}

FVector2D ATile::GetPosition() const
{
	return TileGridPosition;
}


void ATile::Clear()
{
	Status = ETileStatus::EMPTY;
	Obstacle->Destroy();
}

TArray<ATile*> ATile::GetNeighbors()
{
	TArray<ATile*> Neighbors;
	
	AAW_GameMode* GameMode = Cast<AAW_GameMode>(GetWorld()->GetAuthGameMode());
	
	ATile* Neighbour0 = GameMode->GField->GetTileByXYPosition(FVector2D(TileGridPosition.X + 1, TileGridPosition.Y));
	ATile* Neighbour1 = GameMode->GField->GetTileByXYPosition(FVector2D(TileGridPosition.X, TileGridPosition.Y + 1));
	ATile* Neighbour2 = GameMode->GField->GetTileByXYPosition(FVector2D(TileGridPosition.X - 1, TileGridPosition.Y));
	ATile* Neighbour3 = GameMode->GField->GetTileByXYPosition(FVector2D(TileGridPosition.X, TileGridPosition.Y - 1));	
			
	if(Neighbour0) Neighbors.Add(Neighbour0);		
	if(Neighbour1) Neighbors.Add(Neighbour1);		
	if(Neighbour2) Neighbors.Add(Neighbour2);	
	if(Neighbour3) Neighbors.Add(Neighbour3);
	
	return Neighbors;
}

bool ATile::IsMarked() const {return bMark;}

void ATile::Mark() {bMark = true;}

void ATile::Unmark() {bMark = false;}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
	AAW_GameMode* GameMode = Cast<AAW_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->GField->OnResetEvent.AddDynamic(this, &ATile::Clear);
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
