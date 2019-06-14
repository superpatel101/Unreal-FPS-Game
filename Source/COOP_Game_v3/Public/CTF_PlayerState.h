// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SCharacter.h"
#include "CTF_PlayerState.generated.h"

UENUM(BlueprintType)
enum class ETeams : uint8
{
	RedTeam UMETA(DisplayName = "Red Team"),
	BlueTeam UMETA(DisplayName = "Blue Team")
};

/**
 * 
 */
UCLASS()
class COOP_GAME_V3_API ACTF_PlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ACTF_PlayerState();

	UPROPERTY(Replicated)
		int32 TeamNum;
	
	bool IsFriendly(ACTF_PlayerState* A, ACTF_PlayerState* B)
	{
		if (A == nullptr || B == nullptr)
		{
			return true; // assume friendly
		}
		return A->TeamNum == B->TeamNum;
	}
};
