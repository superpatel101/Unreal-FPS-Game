// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/PowerupActor.h"
#include "TimerManager.h"
#include "UnrealNetwork.h"
#include "Engine/Engine.h"

// Sets default values
APowerupActor::APowerupActor()
{
	IntervalBetweenTicks = 0.0f;
	TotalNumberOfTicks = 0;
	SetReplicates(true);
	bIsPowerupActive = false;
}

// Called when the game starts or when spawned
void APowerupActor::BeginPlay()
{
	Super::BeginPlay();
}

void APowerupActor::OnTickPowerup()
{
	OnPowerupTicked(TargetActor);

	TicksCompleted++;
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::FromInt(TicksCompleted));
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::FromInt(TotalNumberOfTicks));

	if (TicksCompleted > TotalNumberOfTicks)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("expired"));
		OnExpired(TargetActor);

		bIsPowerupActive = false;
		OnRep_PowerupActive();
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);

		if (Role == ROLE_Authority)
		{
			Destroy(true);
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("should be deleted...."));
		} else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("NOT SERVER NOT be deleted...."));
		}
	}
}

void APowerupActor::OnRep_PowerupActive()
{
	OnPowerupStateChange(bIsPowerupActive);
}

void APowerupActor::ActivatePowerup(AActor* OtherActor)
{
	TargetActor = OtherActor;
	OnActivated(OtherActor);

	bIsPowerupActive = true;
	OnRep_PowerupActive();
	
	// TimerDelegate.BindUFunction(this, FName("OnTickPowerup"), OtherActor);

	SetActorHiddenInGame(true);

	if (IntervalBetweenTicks > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &APowerupActor::OnTickPowerup, IntervalBetweenTicks, true, 0.0f);
	}
	else
	{
		OnTickPowerup();
	}
}

void APowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APowerupActor, bIsPowerupActive);
}

void APowerupActor::OnPowerupTicked(AActor* OtherActor)
{
}

void APowerupActor::OnActivated(AActor* OtherActor)
{
}

void APowerupActor::OnExpired(AActor* OtherActor)
{
}
