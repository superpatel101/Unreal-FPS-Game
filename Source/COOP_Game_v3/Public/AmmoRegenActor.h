//AmmoRegenActor.h
//2019-6-14
//Header file for the ammo refill powerup
//James Xu, Feeda Elahresh, Aaryan Patel

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
	int32 AmmoAmount = 60;//amount of ammo that it gives
public:
	void OnActivated(AActor* OtherActor) override;//function that deals with the logic of when the ammo pickup is activated
};
