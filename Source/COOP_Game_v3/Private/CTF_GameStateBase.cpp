//CTF_GameStateBase.cpp
//2019-6-14

#include "CTF_GameStateBase.h"
#include "UnrealNetwork.h"

void ACTF_GameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACTF_GameStateBase, RedPoints);

	DOREPLIFETIME(ACTF_GameStateBase, BluePoints);
}
