//BotSWeapon.cpp
//2019-6-14
//Deals with all the weapon logic for the bot
//James Xu, Feeda Elahresh, Aaryan Patel

#include "BotSWeapon.h"
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
#include "TimerManager.h"
#include "BotCharacter.h"

void ABotSWeapon::Fire()
{
	if (Role < ROLE_Authority)
	{
		ServerFire();
	}
	//Trace the world, from pawn eyes to crosshair location
	AActor* MyOwner = GetOwner();


	if (MyOwner)
	{
		ABotCharacter* OwnerChar = Cast<ABotCharacter>(MyOwner);
		if (OwnerChar->GetLoadedAmmo() > 0)
		{
			if (OwnerChar)
			{
				OwnerChar->ReduceAmmoByOne();
			}


			AActor* TargetEnemy = OwnerChar->GetNearestOfClass();


			FVector ShotDirection = TargetEnemy->GetActorLocation() - OwnerChar->GetActorLocation();


			float HalfRad = FMath::DegreesToRadians(BulletSpread);
			ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

			FVector TraceEnd = GetActorLocation() + (ShotDirection * 10000);


			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(MyOwner);
			QueryParams.AddIgnoredActor(this);
			QueryParams.bTraceComplex = true;

			FVector TracerEndPoint = TraceEnd;

			EPhysicalSurface SurfaceType = SurfaceType_Default;

			FHitResult Hit;
			if (GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), TraceEnd, COLLISION_WEAPON, QueryParams))
			{
				//Blocked hit

				AActor* HitActor = Hit.GetActor();
				UGameplayStatics::ApplyPointDamage(HitActor, BaseDamage, ShotDirection, Hit, MyOwner->GetInstigatorController(),
					this, DamageType);

				EPhysicalSurface HitSurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
				PlayImpactEffects(HitSurfaceType, Hit.ImpactPoint);
				PlayFireEffects(TracerEndPoint);
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
