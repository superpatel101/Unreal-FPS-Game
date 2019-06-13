// Fill out your copyright notice in the Description page of Project Settings.


#include "CTF_GameModeBase.h"
#include "SCharacter.h"
#include "TimerManager.h"
#include "COOP_Game_v3.h"
#include "CTF_GameStateBase.h"

ACTF_GameModeBase::ACTF_GameModeBase()
{
    PrimaryActorTick.bCanEverTick = true;
}
void ACTF_GameModeBase::CTFGameStart()
{
	GetWorldTimerManager().SetTimer(TimerHandle_GameEnd, this, &ACTF_GameModeBase::CTFGameEnd, GameDuration);
}

void ACTF_GameModeBase::CTFGameEnd()
{
	//do stuff to end game
}

void ACTF_GameModeBase::StartPlay()
{
	Super::StartPlay();

}



void ACTF_GameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	APawn* Player = NewPlayer->GetPawn();
	ASCharacter* PlayerChar = Cast<ASCharacter>(Player);

	if (PlayerChar)
	{
		PlayerChar->TeamNum = GetNumPlayers() % 2;
	}
}

void ACTF_GameModeBase::FlagCapture(uint8 TeamThatCapturedIt)
{
	ACTF_GameStateBase* GameState = GetGameState<ACTF_GameStateBase>();
	if (GameState)
	{
		if (TeamThatCapturedIt == TEAM_RED)
		{
			GameState->RedPoints++;
		} else if (TeamThatCapturedIt == TEAM_BLUE)
		{
			GameState->BluePoints++;
		}
	}

}

void ACTF_GameModeBase::RestartDeadPlayers()
{
    // Modify function to take in a reference of player controller (APlayerController*)
    UE_LOG(LogTemp, Warning, TEXT("should be respawning"));

    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)//looping through player controllers
    {
        
        APlayerController* PC = It->Get();
        if(PC && PC->GetPawn() == nullptr)//if the pawn is a null pointer; checking if pawn is dead
        {
            RestartPlayer(PC);
        }
    }
    
}

void ACTF_GameModeBase::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
    RestartDeadPlayers();

    
}
