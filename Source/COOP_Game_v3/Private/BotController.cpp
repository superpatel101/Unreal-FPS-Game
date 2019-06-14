//BotController.cpp
//2019-6-14
//Deals with logic of the movement and control of the bot
//James Xu, Feeda Elahresh, Aaryan Patel

#include "BotController.h"
#include "BotCharacter.h"
#include "Engine/Engine.h"
#include "Public/SCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "Public/SWeapon.h"

void ABotController::BeginPlay()
{
    bSetControlRotationFromPawnOrientation = true;
}

AActor* ABotController::GetNearestEnemy()
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

void ABotController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (Role == ROLE_Authority)
    {
		AActor* Nearest = GetNearestEnemy();
        if (Nearest) {
//            K2_SetFocus(Nearest);
            SetFocalPoint(Nearest->GetActorLocation());
            
        } else {
            UE_LOG(LogTemp, Warning, TEXT("(BOTController) Tick failed: Nearest not found"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("(BOTController) Tick failed: not server auth"));
    }
}
