// Fill out your copyright notice in the Description page of Project Settings.


#include "BotCharacter.h"
#include "Engine/Engine.h"
#include "Public/SCharacter.h"
#include "Kismet/GameplayStatics.h"

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
		FVector NearestLoc = Nearest->GetActorLocation();
		FVector Direction = NearestLoc - GetActorLocation();

		Direction.Normalize();
		SetActorRotation(Direction.Rotation());
		MoveForward(10 * DeltaTime);

		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, Direction.Rotation().ToString());
	}

}