//SProjectileWeapon.h
//2019-6-14
//Header file for the grenade launcher
//James Xu, Feeda Elahresh, Aaryan Patel
#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SProjectileWeapon.generated.h"

/**
 *
 */
UCLASS()
class COOP_GAME_V3_API ASProjectileWeapon : public ASWeapon
{
	GENERATED_BODY()

protected:
	void SpawnProjectile();//function to create the projectile in the world when its shot 


	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSpawnProjectile();//server version to do that

	virtual void Fire() override;//this class uses Fire() differently than SWeapon

	UPROPERTY(EditDefaultsOnly, Category = "ProjectileWeapon")
	TSubclassOf<AActor> ProjectileClass;//creates a field to assign a blueprint class to


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* ExplosionEffect;//field to hold the explosion effect


};
