// Fill out your copyright notice in the Description page of Project Settings.


#include "AW_GameInstance.h"

void UAW_GameInstance::IncrementHumanScore()
{
	HumanScore++;
}

void UAW_GameInstance::IncrementCPUScore()
{
	CPUScore++;
}

int32 UAW_GameInstance::GetHumanScore()
{
	return HumanScore;
}

int32 UAW_GameInstance::GetCPUScore()
{
	return CPUScore;
}

void UAW_GameInstance::SetTurnMessage(const FString& Message)
{
	CurrentTurnMessage = Message;
}
