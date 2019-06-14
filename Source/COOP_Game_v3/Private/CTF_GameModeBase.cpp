//CTF_GameModeBase.cpp
//2019-6-14


#include "CTF_GameModeBase.h"
#include "SCharacter.h"
#include "TimerManager.h"
#include "COOP_Game_v3.h"
#include "CTF_GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "TeamBasedPlayerStart.h"
#include "CTF_PlayerState.h"
#include "RedTeamStart.h"
#include "BlueTeamStart.h"
#include "Engine/Engine.h"
#include "CTF_PlayerController.h"

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
	ACTF_PlayerState* State = NewPlayer->GetPlayerState<ACTF_PlayerState>();
	APawn* Player = NewPlayer->GetPawn();
	ASCharacter* PlayerChar = Cast<ASCharacter>(Player);
	ACTF_PlayerController* Controller = Cast<ACTF_PlayerController>(NewPlayer);

	if (ensure(State))
	{

		if (RedPlayerCount <= BluePlayerCount)
		{
			RedPlayerCount++;
			State->TeamNum = TEAM_RED;
			
			if (PlayerChar)
			{
				PlayerChar->TeamNum = TEAM_RED;
			}
			if (Controller)
			{
				Controller->TeamNum = TEAM_RED;
			}
			
		} else
		{
			BluePlayerCount++;
			State->TeamNum = TEAM_BLUE;
			if (PlayerChar)
			{
				PlayerChar->TeamNum = TEAM_BLUE;
			}

			if (Controller)
			{
				Controller->TeamNum = TEAM_BLUE;
			}
		}
		UE_LOG(LogTemp, Warning,TEXT("Red: %d, Blue: %d"), RedPlayerCount, BluePlayerCount);
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("NO PLAYER STATE"));
	}

	/*
	APawn* Player = NewPlayer->GetPawn();
	ASCharacter* PlayerChar = Cast<ASCharacter>(Player);

	if (PlayerChar)
	{
		PlayerChar->TeamNum = GetNumPlayers() % 2;
		UE_LOG(LogTemp, Error, TEXT("WARNING: CALLED MULTIPLE TIMES!!!"));
	}
	*/
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
    // UE_LOG(LogTemp, Warning, TEXT("should be respawning"));

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

AActor* ACTF_GameModeBase::ChoosePlayerStart(AController* Player)
{
	Super::ChoosePlayerStart_Implementation(Player);
	ACTF_PlayerState* PlayerState = Cast<ACTF_PlayerState>(Player->PlayerState);
	ACTF_PlayerController* Controller = Cast<ACTF_PlayerController>(Player);
	// Player->PlayerState->bIsABot;
	if (Controller)
	{
		if (Controller->TeamNum == TEAM_RED)
		{
			TArray<AActor*> PlayerStarts;

			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARedTeamStart::StaticClass(), PlayerStarts);

			if (PlayerStarts.Num() != 1)
			{
				UE_LOG(LogTemp, Error, TEXT("TOO MANY STARTS!"));
			}
			UE_LOG(LogTemp, Error, TEXT("Sea of Red"));

			return PlayerStarts[0];
		}
		else //(Controller->TeamNum == TEAM_BLUE)
		{
			TArray<AActor*> PlayerStarts;

			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlueTeamStart::StaticClass(), PlayerStarts);

			if (PlayerStarts.Num() != 1)
			{
				UE_LOG(LogTemp, Error, TEXT("TOO MANY STARTS!"));
			}

			return PlayerStarts[1];
		}	

	} else
	{
		UE_LOG(LogTemp, Error, TEXT("NULL PLAYER CONTROLLER!!!!"))
	}
	return nullptr;
}
