//SProjectileWeapon.cpp
//2019-6-14
//Deals with some of the logic of the grenade launcher, part of this weapon was built using blueprints in unreal engine
//James Xu, Feeda Elahresh, Aaryan Patel
#include "SProjectileWeapon.h"
#include "Engine/Engine.h"
#include "SCharacter.h"


void ASProjectileWeapon::SpawnProjectile()//when a projectile is shot, it spawns the actor
{
	if (Role == ROLE_Authority)
	{
		AActor* MyOwner = GetOwner();//gets the current actor
		if (MyOwner && ProjectileClass) {//if there is an assigned projectile class and an owner
			ASCharacter* MyChar = Cast<ASCharacter>(MyOwner);//we cast the owner object to an SCharacter object from our SCharacter class
			if (MyChar)//if there is a character object
			{
				MyChar->ReduceAmmoByOne();//reduce ammo
			}
			FVector EyeLocation;//eyelocation vector
			FRotator EyeRotation;//eye rotation vector
			MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);//gets the viewpoint from the previous info

			FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);//gets the muzzlelocation vector
			FRotator MuzzleRotation = MeshComp->GetSocketRotation(MuzzleSocketName);
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


			GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, EyeRotation, SpawnParams);//just spawns the projectile

		}
	} else//if not client it'll spawn for server
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





void ASProjectileWeapon::Fire() {//this function is overrided from the SWeapon Fire() because this gun spawns an actual projectile
	SpawnProjectile();
}

