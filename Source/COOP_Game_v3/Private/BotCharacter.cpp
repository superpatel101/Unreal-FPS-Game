// Fill out your copyright notice in the Description page of Project Settings.


#include "BotCharacter.h"
#include "Engine/Engine.h"
#include "Public/SCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "Public/SWeapon.h"

void ABotCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    if (Role == ROLE_Authority)
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
                if ((Nearest->GetActorLocation() - GetActorLocation()).Size() > 300)
                {
                    MoveForward(80 * DeltaTime);
                }
            }
            if (GetWorld()->GetRealTimeSeconds() - TimeLastFired > 1)
            {
                Fire();
                TimeLastFired = GetWorld()->GetRealTimeSeconds();
            }
        }
        
    } else
    {
//        UE_LOG(LogTemp, Warning, TEXT("(BOT) Tick failed: not ROLE_Authority"));

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

void ABotCharacter::ServerBeginPlay_Implementation()
{
    BeginPlay();
}

bool ABotCharacter::ServerBeginPlay_Validate()
{
    return true;
}
