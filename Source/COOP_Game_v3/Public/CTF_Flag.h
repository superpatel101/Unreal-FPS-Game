//CTF_Flag.h
//2019-6-14
//Header file for the capture the flag suitcase pickup, will keep track of whether the person has taken the flag and brought it to their team's side
//James Xu, Feeda Elahresh, Aaryan Patel
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "CTFPickup.h"
#include "CTF_PlayerState.h"
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

	void AttachFlagToPlayer(ASCharacter* Player);//function to attach flag to player

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAttachFlagToPlayer(ASCharacter* Player);//server version

	void Detach();//detach flag once destination is reached

	UFUNCTION(Server, Reliable, WithValidation)//server version
		void ServerDetach();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerBeginPlay();//server version

	UStaticMeshComponent* MeshComp;//mesh component for how the flag looks (suitcase)

	UPROPERTY(Replicated)
	ACTFPickup* Destination;//object which shows where the destination is

	void OnReachDest();//function that handles the event when destination is reached

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Replicated)
	int32 TeamNum;//team number
};
