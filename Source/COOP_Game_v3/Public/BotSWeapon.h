//BotSWeapon.h
//2019-6-14
//Header file for the SWeapon class but for the bots
//James Xu, Feeda Elahresh, Aaryan Patel
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
	void Fire() override;//overrides fire for the bot weapon

	//Bullet spread in degrees
	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin=0.0f))
	float BulletSpread = 2.0f;
};
