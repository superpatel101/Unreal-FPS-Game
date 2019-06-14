//HealthRegenActor.h
//2019-6-14
//Header file for the health regen powerup
//James Xu, Feeda Elahresh, Aaryan Patel
#pragma once

#include "CoreMinimal.h"
#include "PowerupActor.h"
#include "HealthRegenActor.generated.h"

/**
 * 
 */
UCLASS()
class COOP_GAME_V3_API AHealthRegenActor : public APowerupActor
{
	GENERATED_BODY()
public:
	AHealthRegenActor();//constructor
private:
	float regenPerTick;//how much health is added per tick
public:
	void OnPowerupTicked(AActor* OtherActor) override;//tick function when it's on the powerup
	void OnActivated(AActor* OtherActor) override;//when it's activated
	void OnExpired(AActor* OtherActor) override;//and when it ends
};
