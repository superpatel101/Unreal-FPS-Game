// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CTF_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class COOP_GAME_V3_API ACTF_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated)
	int32 TeamNum;

public:
	ACTF_PlayerController();
};
