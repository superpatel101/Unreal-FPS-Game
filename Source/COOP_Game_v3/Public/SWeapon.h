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
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;//just the mesh component parameter to pass in the gun mesh

	void PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint);//this will play impact affects where the bullet hits
	void PlayFireEffects(FVector TraceEnd);//

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
	virtual void Fire();



	FTimerHandle TimerHandle_TimeBetweenShots;

	float LastFireTime;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Weapon")
	float RateOfFire;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetFireRate(float FireRate);

	float TimeBetweenShots;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = 0.0f))
		float BaseDamage;

	UFUNCTION()
		void OnRep_HitScanTrace();

	

public:
	
	void StartFire();
	void StopFire();
	void SetFireRate(float FireRate);
	float GetFireRate();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFire();

};
