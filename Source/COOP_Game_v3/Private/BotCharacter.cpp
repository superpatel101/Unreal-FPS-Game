// Fill out your copyright notice in the Description page of Project Settings.


#include "BotCharacter.h"
#include "Engine/Engine.h"
#include "Public/SCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "Public/SWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Public/HealthRegenActor.h"
#include "COOP_Game_v3.h"
#include "AmmoRegenActor.h"

uint32 ABotCharacter::GetDecision()
{
	if (LoadedAmmo == 0)
	{
		Reload();
	}
	if (HealthComponent->GetHealth() <= 30)
	{
		return RETREAT;
	}

	AActor* Nearest = GetNearestOfClass();

	if(LoadedAmmo > 0)
	{
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(Nearest);
		QueryParams.bTraceComplex = true;

		FHitResult Hit;

		if (GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), Nearest->GetActorLocation(), COLLISION_WEAPON, QueryParams))
		{
			return MOVE_TOWARDS_ENEMY;
		}
		else
		{
			Attack();
		}
	} else
	{
		return AMMO_REFILL;
	}
	
	if (Nearest->GetDistanceTo(this) > MIN_DIST)
	{
		return MOVE_TOWARDS_ENEMY;
	}
	// return AMMO_REFILL;
	

	return -1;
}

void ABotCharacter::MoveTowardsEnemy(float DeltaTime, AActor* Target)
{
	AActor* Nearest;
	if (Target)
	{
		Nearest = Target;
	} else
	{
		Nearest = GetNearestOfClass();

	}
	if (Nearest)
	{
		UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), Nearest);

		if (NavPath && NavPath->PathPoints.Num() > 1)
		{
			FVector NextPoint = NavPath->PathPoints[1];
			FVector Direction = NextPoint - GetActorLocation();
			Direction.Normalize();
			//                GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, GetControlRotation().ToString());
			FRotator Rotation = Direction.Rotation();
			Rotation.Pitch = 0;
			Rotation.Roll = 0;
			SetActorRotation(Rotation);
			//if ((Nearest->GetActorLocation() - GetActorLocation()).Size() > MIN_DIST)
			//{
			MoveForward(80 * DeltaTime);
			//}
		}
	}
}

void ABotCharacter::GoToHealthRegen(float DeltaTime)
{
	AActor* Nearest = GetNearestOfClass(AHealthRegenActor::StaticClass());
	MoveTowardsEnemy(DeltaTime, Nearest);
}

void ABotCharacter::GoToAmmoRefill(float DeltaTime)
{
	AActor* Nearest = GetNearestOfClass(AAmmoRegenActor::StaticClass());
	MoveTowardsEnemy(DeltaTime, Nearest);
}


void ABotCharacter::Attack()
{
	if (GetWorld()->GetRealTimeSeconds() - TimeLastFired > 0.4)
	{
		CurrentWeapon->Fire();
		TimeLastFired = GetWorld()->GetRealTimeSeconds();
	}
}

void ABotCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
	if (Role == ROLE_Authority)
	{
		const uint32 Decision = GetDecision();
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::FromInt(Decision));

		if (Decision == AMMO_REFILL)
		{
			GoToAmmoRefill(DeltaTime);
		}
		else if (Decision == MOVE_TOWARDS_ENEMY)
		{
			StopFire();
			MoveTowardsEnemy(DeltaTime);
		}
		else if (Decision == RETREAT)
		{
			StopFire();
			GoToHealthRegen(DeltaTime);
		}
		if (bShouldFire)
		{
			StartFire();
			bShouldFire = false;
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("STARTING FIRE"));

		}
		if (bShouldStopFire)
		{
			StopFire();
			bShouldStopFire = false;
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("STOPPING FIRE"));

		}
	}
}

void ABotCharacter::BeginPlay()
{
    Super::BeginPlay();
    if (Role != ROLE_Authority)
    {
        ServerBeginPlay();
    } else
    {
        TimeLastFired = GetWorld()->GetRealTimeSeconds();
        
        UE_LOG(LogTemp, Warning, TEXT("Now server!"));
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        
        
        CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        
        if (CurrentWeapon) {
            
            CurrentWeapon->SetOwner(this);
            CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("Current weapon not found!"));
        }
    }
}

AActor* ABotCharacter::GetNearestOfClass(UClass* Type)
{
	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Type, FoundActors);

	AActor* Nearest = nullptr;

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		if (FoundActors[i] != this && !Cast<ABotCharacter>(FoundActors[i]))
		{
			if (!Nearest)
			{
				Nearest = FoundActors[i];
			}
			float distOld = Nearest->GetDistanceTo(this);
			float distNew = FoundActors[i]->GetDistanceTo(this);

			if (distNew < distOld)
			{
				Nearest = FoundActors[i];
			}
		}
	}
	return Nearest;
}

void ABotCharacter::ServerBeginPlay_Implementation()
{
    BeginPlay();
}

bool ABotCharacter::ServerBeginPlay_Validate()
{
    return true;
}
