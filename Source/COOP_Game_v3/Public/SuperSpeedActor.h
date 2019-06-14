//SuperSpeedActor.h
//2019-6-14
//Header file for the speed boost pickup
//James Xu, Feeda Elahresh, Aaryan Patel
#pragma once

#include "CoreMinimal.h"
#include "PowerupActor.h"
#include "SuperSpeedActor.generated.h"

/**
 * 
 */
UCLASS()
class COOP_GAME_V3_API ASuperSpeedActor : public APowerupActor
{
	GENERATED_BODY()
public:
	ASuperSpeedActor();
private:
	float speedMultiplier;//will hold the value of how much faster the player gets when they go over the powerup
public:
	void OnPowerupTicked(AActor* OtherActor) override;
	void OnActivated(AActor* OtherActor) override;
	void OnExpired(AActor* OtherActor) override;
	
};
