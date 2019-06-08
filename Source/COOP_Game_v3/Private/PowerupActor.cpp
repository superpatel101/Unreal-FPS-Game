// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/PowerupActor.h"
#include "TimerManager.h"
#include "UnrealNetwork.h"

// Sets default values
APowerupActor::APowerupActor()
{
	PowerupInterval = 0.0f;
	TotalTicks = 0;
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
	OnPowerupTicked();

	TicksDone++;

	if (TicksDone >= TotalTicks)
	{
		OnExpired();
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}

void APowerupActor::OnRep_PowerupActive()
{
	OnPowerupStateChange(bIsPowerupActive);
}

void APowerupActor::ActivatePowerup()
{
	bIsPowerupActive = true;
	OnActivated();
	if (PowerupInterval > 0.0f)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &APowerupActor::OnTickPowerup, PowerupInterval,
		                                true);
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
