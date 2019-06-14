//CTF_GameStateBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CTF_GameStateBase.generated.h"




/**
 * 
 */
UCLASS()
class COOP_GAME_V3_API ACTF_GameStateBase : public AGameStateBase
{
	GENERATED_BODY()
protected:

public:

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 RedPoints = 0;

	UPROPERTY(Replicated, BlueprintReadOnly)
	int32 BluePoints = 0;
};
