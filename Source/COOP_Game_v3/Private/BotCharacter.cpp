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

uint32 ABotCharacter::GetDecision()
{
	return MOVE_TOWARDS_ENEMY;
}

void ABotCharacter::MoveTowardsEnemy(float DeltaTime, AActor* Target)
{
	AActor* Nearest;
	if (Target)
	{
		Nearest = Target;
	} else
	{
		Nearest = GetNearestEnemy();

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
			if ((Nearest->GetActorLocation() - GetActorLocation()).Size() > MIN_DIST)
			{
				MoveForward(80 * DeltaTime);
			}
		}
	}
}

void ABotCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
	if (Role == ROLE_Authority)
	{
		const uint32 Decision = GetDecision();

		if (Decision == SHOOT)
		{

		}
		else if (Decision == MOVE_TOWARDS_ENEMY)
		{
			MoveTowardsEnemy(DeltaTime);
		}
		else if (Decision == RETREAT)
		{

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

AActor* ABotCharacter::GetNearestEnemy()
{
	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCharacter::StaticClass(), FoundActors);

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
