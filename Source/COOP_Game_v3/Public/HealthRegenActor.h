// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerupActor.h"
#include "HealthRegenActor.generated.h"

/**
 * 
 */
UCLASS()
class COOP_GAME_V3_API AHealthRegenActor : public APowerupActor
{
	GENERATED_BODY()
public:
	AHealthRegenActor();
private:
	float regenPerTick;
public:
	void OnPowerupTicked(AActor* OtherActor) override;
	void OnActivated(AActor* OtherActor) override;
	void OnExpired(AActor* OtherActor) override;
};
