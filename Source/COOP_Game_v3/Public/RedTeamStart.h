// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "RedTeamStart.generated.h"

/**
 * 
 */
UCLASS()
class COOP_GAME_V3_API ARedTeamStart : public APlayerStart
{
	GENERATED_BODY()
public:
	int32 TeamNum = 0;
};
