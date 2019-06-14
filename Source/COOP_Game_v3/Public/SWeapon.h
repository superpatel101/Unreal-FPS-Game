//SWeapon.h
//2019-6-14
//Header file for the base weapon class (rifle). This exposes the weapon to the unreal engine and initializes any classes, functions, or fields that'll be used
//James Xu, Feeda Elahresh, Aaryan Patel

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletalMeshComponent;//these are classes used

class DamageType;
class UParticleSystem;
class UDamageType;

//Contains info of a single hitscan weapon linetrace
USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

public:

	UPROPERTY()//these are just fields that are made 
		TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY()
		FVector_NetQuantize TraceTo;

	UPROPERTY()
		float UniqueValue;
};


UCLASS()
class COOP_GAME_V3_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

protected:
	
	virtual void BeginPlay() override;//plays at the beginning

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;//just the mesh component parameter to pass in the gun mesh

	void PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint);//this will play impact affects where the bullet hits
	void PlayFireEffects(FVector TraceEnd);//function that plays all fire effects

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;//this is a field made to determine the damage type of the weapon, for the rifle it is a point damage

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "Weapon")
	FName MuzzleSocketName;//this field will point to a socket on the gun mesh where the flash will attach to

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTargetName;//target name, will determine if the thing the bullet hits is supposed to get damaged

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")//these 3 things just hold the effects for firing
	UParticleSystem* MuzzleEffect;//flash at muzzle

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* ImpactEffect;//mini explosion/sparks on hit

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* TracerEffect;//smokey path

	UPROPERTY(ReplicatedUsing = OnRep_HitScanTrace)
		FHitScanTrace HitScanTrace;

	UFUNCTION(BlueprintCallable)
	virtual void Fire();//this function holds all logic of firing the gun



	FTimerHandle TimerHandle_TimeBetweenShots;//makes sure their is a certain amount of time between shots

	float LastFireTime;//value of last time a shot was fired

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Weapon")
	float RateOfFire;//field that determines rate of fire (shown in unreal editor)

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetFireRate(float FireRate);//server version of above

	float TimeBetweenShots;//initializes time between shots

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = 0.0f))
		float BaseDamage;//field showing base damage of weapon

	UFUNCTION()
		void OnRep_HitScanTrace();//it calls the function whenever hit scan trace changes

	

public:
	
	void StartFire();//when fire button is pressed
	void StopFire();//when released
	void SetFireRate(float FireRate);//setter for the fire rate
	float GetFireRate();//getter

	UFUNCTION(Server, Reliable, WithValidation)//function that makes it if the actor doesn't have server authority this just runs on the server 
		void ServerFire();

};
