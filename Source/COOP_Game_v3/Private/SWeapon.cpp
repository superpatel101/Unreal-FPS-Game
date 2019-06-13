// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/SWeapon.h"
#include "DrawDebugHelpers.h"
#include "../Public/SCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "UnrealNetwork.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "COOP_Game_v3.h"
#include "Engine/Engine.h"
#include "TimerManager.h"



static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVarDebugWeaponDrawing(
	TEXT("COOP.DebugWeapons"),
	DebugWeaponDrawing,
	TEXT("Draw Debug Lines For Weapons"),
	ECVF_Cheat);


// Sets default values - constructor
ASWeapon::ASWeapon()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;//we make the main mesh the MeshComp

	MuzzleSocketName = "MuzzleSocket";//In the UE4 editor we've assigned the end of the gun as the Muzzle Socket to allow animations to come from
	TracerTargetName = "Target";
	
	SetReplicates(true);

	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;
	BaseDamage = 10.0f;//base damage
	//RateOfFire = 600;
}

void ASWeapon::BeginPlay()//runs in the beginning of play once
{
	Super::BeginPlay();
	TimeBetweenShots = 1/RateOfFire;
}


void ASWeapon::OnRep_HitScanTrace()
{
	// Play cosmetic FX
	PlayFireEffects(HitScanTrace.TraceTo);
	PlayImpactEffects(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);
}

void ASWeapon::StartFire()
{
	float FirstDelay = FMath::Max(LastFireTime +TimeBetweenShots- GetWorld()->TimeSeconds,0.0f);//this makes sure their is a minimum delay between two clicks for shots to happen
	//the FirstDelay makes sure users can't spam click as that could be faster than the set firerate
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ASWeapon::Fire, TimeBetweenShots, true,0.0f);//sets the delay
}

void ASWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

void ASWeapon::SetFireRate(float FireRate)
{
	if (Role == ROLE_Authority)
	{
		RateOfFire = FireRate;
	} else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, "HUHUHUHUH");
		
	}
}

void ASWeapon::ServerSetFireRate_Implementation(float FireRate)
{
	SetFireRate(FireRate);
}

bool ASWeapon::ServerSetFireRate_Validate(float FireRate)
{
	return true;
}



float ASWeapon::GetFireRate()//getter for firerate
{
	return RateOfFire;
}




void ASWeapon::Fire()//this function deals with firing for client and server
{

	if (Role < ROLE_Authority)//if serverside
	{
		ServerFire();//calls the server fire 
	}
	//Trace the world, from pawn eyes to crosshair location
	AActor* MyOwner = GetOwner();//otherwise we get the actor



	if (MyOwner)
	{
		ASCharacter* OwnerChar = Cast<ASCharacter>(MyOwner);
		if (OwnerChar->GetLoadedAmmo() > 0)//if there is ammo for the user to use
		{
			if (OwnerChar)
			{
				OwnerChar->ReduceAmmoByOne();
			}
			FVector EyeLocation;
			FRotator EyeRotation;


			MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);


			FVector ShotDirection = EyeRotation.Vector();//the direction of the shot is just where the user is looking

			FVector TraceEnd = EyeLocation + (ShotDirection * 10000);//for the line trace, we just make a very long vector which will cover the entire map so it reaches everything that it could aim


			FCollisionQueryParams QueryParams;//this object makes sure the bullet ignores collision with the gun itself and the player
			QueryParams.AddIgnoredActor(MyOwner);
			QueryParams.AddIgnoredActor(this);
			QueryParams.bTraceComplex = true;

			FVector TracerEndPoint = TraceEnd;//endpoint of traceend

			EPhysicalSurface SurfaceType = SurfaceType_Default;

			FHitResult Hit;//a variable that will hold the HitResult of the shot
			//traces a ray against the worls and returns the first hit
			if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams))
			{
				//Blocked hit

				AActor* HitActor = Hit.GetActor();//gets the actor of what was hit
				UGameplayStatics::ApplyPointDamage(HitActor, BaseDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(),
					this, DamageType);//apply point damage to the actor (single bullet)
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, "It's Applying Damage");

<<<<<<< HEAD
				EPhysicalSurface HitSurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
				PlayImpactEffects(HitSurfaceType, Hit.ImpactPoint);
				PlayFireEffects(TracerEndPoint);
=======
				EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
				PlayImpactEffects(SurfaceType, Hit.ImpactPoint);
				PlayFireEffects(TracerEndPoint);//plays an effect where the bullet hits
>>>>>>> b8bb097759534ea2de892ae213104be851fdc506
			}

			if (DebugWeaponDrawing > 0)
			{
				DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Red, false, 1.0f, 0, 1.0f);
			}

			TracerEndPoint = Hit.ImpactPoint;

			LastFireTime = GetWorld()->TimeSeconds;

			if (Role == ROLE_Authority)
			{
				HitScanTrace.TraceTo = TracerEndPoint;
				HitScanTrace.SurfaceType = SurfaceType;
				HitScanTrace.UniqueValue = UGameplayStatics::GetRealTimeSeconds(GetWorld());
			}
		}
	}

}

void ASWeapon::ServerFire_Implementation()
{
	Fire();
}

bool ASWeapon::ServerFire_Validate()
{
	return true;
}



void ASWeapon::PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint)//plays the impact effects
{
	UParticleSystem* SelectedEffect;
	//switch (SurfaceType)
	//{
	//default:
	SelectedEffect = MuzzleEffect;//we 
	//}
	if (SelectedEffect)
	{
		FVector ShotDirection = ImpactPoint - MeshComp->GetSocketLocation(MuzzleSocketName);
		ShotDirection.Normalize();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, ImpactPoint, ShotDirection.Rotation());
	}
}

void ASWeapon::PlayFireEffects(FVector TraceEnd)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}


	if (TracerEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComp)
		{
			TracerComp->SetVectorParameter(TracerTargetName, TraceEnd);
		}
	}
}

void ASWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASWeapon, HitScanTrace, COND_SkipOwner);
}
