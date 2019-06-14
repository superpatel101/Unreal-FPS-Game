// Fill out your copyright notice in the Description page of Project Settings.


#include "CTF_PlayerState.h"
#include "UnrealNetwork.h"

void ACTF_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACTF_PlayerState, TeamNum);
}

ACTF_PlayerState::ACTF_PlayerState()
{
	SetReplicates(true);
}
