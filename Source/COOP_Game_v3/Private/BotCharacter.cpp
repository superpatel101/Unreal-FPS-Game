// Fill out your copyright notice in the Description page of Project Settings.


#include "BotCharacter.h"
#include "Engine/Engine.h"
#include "Public/SCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"

void ABotCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCharacter::StaticClass(), FoundActors);

	AActor* Nearest = nullptr;

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		if (FoundActors[i] != this)
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
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, GetControlRotation().ToString());
			FRotator Rotation = Direction.Rotation();
			Rotation.Pitch = 0;
			Rotation.Roll = 0;
			SetActorRotation(Rotation);
			MoveForward(5);
		}
		
		FVector NearestLoc = Nearest->GetActorLocation();
		FVector Direction = NearestLoc - GetActorLocation();

		Direction.Normalize();
		

	}

}
