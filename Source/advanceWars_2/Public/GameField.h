// Source/advanceWars_2/Public/GameField.h

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.h"
#include "GameField.generated.h"

UCLASS()
class ADVANCEWARS_2_API AGameField : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AGameField();

	UPROPERTY(Transient)
	TArray<ATile*> Tiles;

	UPROPERTY(Transient)
	TMap<FVector2D, ATile*> TileMap;
	
	static const int32 NOT_ASSIGNED = -1;

	// UPROPERTY(BlueprintAssignable)
	// FOnReset OnResetEvent;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Field Config")
	int32 Size;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Field Config")
	float TileSize;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> TileClass_Odd;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATile> TileClass_Even;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable)
	void ResetField();

	void GenerateField();

	FVector2D GetPosition(const FHitResult& Hit);

	FVector GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const;
	
	FVector2D GetXYPositionByRelativeLocation(const FVector& Location) const;

	

	

	
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
