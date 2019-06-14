// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "BlueTeamStart.generated.h"

/**
 * 
 */
UCLASS()
class COOP_GAME_V3_API ABlueTeamStart : public APlayerStart
{
	GENERATED_BODY()

public:
	int32 TeamNum = 1;
};
