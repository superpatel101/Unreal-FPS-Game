// Fill out your copyright notice in the Description page of Project Settings.


#include "CTF_PlayerController.h"
#include "UnrealNetwork.h"

ACTF_PlayerController::ACTF_PlayerController()
{
}

void ACTF_PlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACTF_PlayerController, TeamNum);
}