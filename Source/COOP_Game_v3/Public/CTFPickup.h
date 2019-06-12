// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTF_Flag.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "SCharacter.h"

#include "CTFPickup.generated.h"

class ASCharacter;
class ACTF_Flag;
class USphereComponent;
class UDecalComponent;

UCLASS()
class COOP_GAME_V3_API ACTFPickup : public AActor
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UDecalComponent* DecalComp;

	UPROPERTY(EditDefaultsOnly, Category = "Flag")
		TSubclassOf<ACTF_Flag> PowerUpClass;

	UPROPERTY(EditDefaultsOnly, Category = "Flag")
		float CoolDownDuration;

	FTimerHandle RespawnTimer;

	ACTFPickup();

	UPROPERTY(Replicated)
	ACTF_Flag* FlagInstance;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Flag")
	int32 TeamNum;

	void GiveFlag(ASCharacter* Target);
public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void Respawn();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRespawn();

	int32 GetTeamNum()
	{
		return TeamNum;
	}
};