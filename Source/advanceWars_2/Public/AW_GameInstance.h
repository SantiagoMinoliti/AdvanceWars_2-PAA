// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AW_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEWARS_2_API UAW_GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int32 HumanScore = 0;
	
	UPROPERTY(EditAnywhere)
	int32 CPUScore = 0;
	
	UPROPERTY(EditAnywhere)
	FString CurrentTurnMessage = "Current Player";

	void IncrementHumanScore();
	
	void IncrementCPUScore();

	UFUNCTION(BlueprintCallable)
	int32 GetHumanScore();
	
	UFUNCTION(BlueprintCallable)
	int32 GetCPUScore();

	void SetTurnMessage(const FString& Message);
	
	
};
