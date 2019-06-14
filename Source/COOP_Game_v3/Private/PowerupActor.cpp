//PowerupActor.cpp
//2019-6-14
//General class powerups are derived from
////James Xu, Feeda Elahresh, Aaryan Patel
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

void APowerupActor::OnTickPowerup()//when it's powered up
{
	OnPowerupTicked(TargetActor);

	TicksCompleted++;
	
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
		} else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("NOT SERVER NOT be deleted...."));
		}
	}
}

void APowerupActor::OnRep_PowerupActive()//changes the state so the powerup is active or inactive
{
	OnPowerupStateChange(bIsPowerupActive);
}

void APowerupActor::ActivatePowerup(AActor* OtherActor)//activates powerup
{
	TargetActor = OtherActor;//target actor is whoever hits the powerup
	OnActivated(OtherActor);

	bIsPowerupActive = true;//sets to true because its now active
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
