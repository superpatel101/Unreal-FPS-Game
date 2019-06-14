//SProjectileWeapon.h

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
	void SpawnProjectile();


	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSpawnProjectile();

	virtual void Fire() override;//this class uses Fire() differently than SWeapon

	UPROPERTY(EditDefaultsOnly, Category = "ProjectileWeapon")
	TSubclassOf<AActor> ProjectileClass;//creates a field to assign a blueprint class to


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* ExplosionEffect;


};
