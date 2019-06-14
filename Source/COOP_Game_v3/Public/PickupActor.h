//PickupActor.h
//2019-6-14
//Header file for the pickups (powerups). The classes concerning the powerups dealt with the logic, this deals with the actual picking it up
//James Xu, Feeda Elahresh, Aaryan Patel
#pragma once

#include "CoreMinimal.h"//imports used
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PowerupActor.h"
#include "PickupActor.generated.h"

class USphereComponent;
class UDecalComponent;
class APowerupActor;

UCLASS()
class COOP_GAME_V3_API APickupActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")//sphere collision component field
		USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")//decal design field
		UDecalComponent* DecalComp;

	UPROPERTY(EditDefaultsOnly, Category = "PickupActor")//field that holds what powerup class it is for unreal
	TSubclassOf<APowerupActor> PowerUpClass;

	UPROPERTY(EditDefaultsOnly, Category = "PickupActor")//cool down duration for when the pickup ends
	float CoolDownDuration;

	FTimerHandle RespawnTimer;//handles the time for when the powerup respawns

	void Respawn();//function that respawns the powerup

	APowerupActor* PowerupInstance;//object that holds the powerup instance
public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;//notifies whether another actor is overlapped with the powerup
};