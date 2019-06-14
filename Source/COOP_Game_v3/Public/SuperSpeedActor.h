//SuperSpeedActor.h

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
	float speedMultiplier;
public:
	void OnPowerupTicked(AActor* OtherActor) override;
	void OnActivated(AActor* OtherActor) override;
	void OnExpired(AActor* OtherActor) override;
	
};
