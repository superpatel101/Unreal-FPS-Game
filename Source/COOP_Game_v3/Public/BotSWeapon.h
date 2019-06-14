//BotSWeapon.h

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "BotSWeapon.generated.h"

/**
 * 
 */
UCLASS()
class COOP_GAME_V3_API ABotSWeapon : public ASWeapon
{
	GENERATED_BODY()

protected:
	void Fire() override;

	//Bullet spread in degrees
	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin=0.0f))
	float BulletSpread = 2.0f;
};
