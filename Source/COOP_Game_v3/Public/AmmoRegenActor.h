//AmmoRegenActor.h

#pragma once

#include "CoreMinimal.h"
#include "PowerupActor.h"
#include "AmmoRegenActor.generated.h"

/**
 * 
 */
UCLASS()
class COOP_GAME_V3_API AAmmoRegenActor : public APowerupActor
{
	GENERATED_BODY()
private:
	int32 AmmoAmount = 60;
public:
	// void OnPowerupTicked(AActor* OtherActor) override;
	void OnActivated(AActor* OtherActor) override;
	// void OnExpired(AActor* OtherActor) override;
};
