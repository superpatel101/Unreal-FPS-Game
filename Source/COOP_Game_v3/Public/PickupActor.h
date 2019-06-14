//PickupActor.h

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UDecalComponent* DecalComp;

	UPROPERTY(EditDefaultsOnly, Category = "PickupActor")
	TSubclassOf<APowerupActor> PowerUpClass;

	UPROPERTY(EditDefaultsOnly, Category = "PickupActor")
	float CoolDownDuration;

	FTimerHandle RespawnTimer;

	void Respawn();

	APowerupActor* PowerupInstance;
public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};