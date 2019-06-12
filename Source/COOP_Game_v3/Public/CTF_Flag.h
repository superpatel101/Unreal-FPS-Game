// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "CTFPickup.h"
#include "CTF_Flag.generated.h"

class ASCharacter;
class UStaticMeshComponent;
class ACTFPickup;

UCLASS()
class COOP_GAME_V3_API ACTF_Flag : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACTF_Flag();

	void AttachFlagToPlayer(ASCharacter* Player);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttachFlagToPlayer(ASCharacter* Player);

	void Detach();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerDetach();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerBeginPlay();

	UStaticMeshComponent* MeshComp;

	UPROPERTY(Replicated)
	ACTFPickup* Destination;

	void OnReachDest();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Replicated)
	int32 TeamNum;
};
