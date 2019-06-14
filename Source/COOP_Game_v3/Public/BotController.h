//BotController.h
//2019-6-14
//Header file for the movement of the bot
//James Xu, Feeda Elahresh, Aaryan Patel
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BotController.generated.h"

/**
 * 
 */
UCLASS()
class COOP_GAME_V3_API ABotController : public AAIController
{
	GENERATED_BODY()
public:
    void Tick(float DeltaTime) override;
    void BeginPlay() override;
	AActor* GetNearestEnemy();
};
