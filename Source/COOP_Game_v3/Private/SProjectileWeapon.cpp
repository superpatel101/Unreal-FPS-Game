// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileWeapon.h"
#include "Engine/Engine.h"
#include "SCharacter.h"


void ASProjectileWeapon::SpawnProjectile()
{
	if (Role == ROLE_Authority)
	{
		AActor* MyOwner = GetOwner();
		if (MyOwner && ProjectileClass) {
			ASCharacter* MyChar = Cast<ASCharacter>(MyOwner);
			if (MyChar)
			{
				MyChar->ReduceAmmoByOne();
			}
			FVector EyeLocation;
			FRotator EyeRotation;
			MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

			FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
			FRotator MuzzleRotation = MeshComp->GetSocketRotation(MuzzleSocketName);
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


			GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, EyeRotation, SpawnParams);

		}
	} else
	{
		ServerSpawnProjectile();
	}

}

void ASProjectileWeapon::ServerSpawnProjectile_Implementation()
{
	SpawnProjectile();
}

bool ASProjectileWeapon::ServerSpawnProjectile_Validate()
{
	return true;
}



void ASProjectileWeapon::Fire() {
	SpawnProjectile();
}

