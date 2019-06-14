//PowerupActor.h
//2019-6-14
//Header file for the powerup class, will expose the PowerupActor.cpp class to unreal engine, which both powerups use 
//James Xu, Feeda Elahresh, Aaryan Patel
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerupActor.generated.h"

UCLASS()
	class COOP_GAME_V3_API APowerupActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APowerupActor();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	float IntervalBetweenTicks;//just the time interval between ticks

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 TotalNumberOfTicks;//keeps track of total tick numbers

	//Total # of ticks applied
	int32 TicksCompleted;

	FTimerHandle TimerHandle_PowerupTick;//handles the time for the power up to last

	UFUNCTION()
	void OnTickPowerup();//function that happens every frame keeping track of power up 

	UPROPERTY(ReplicatedUsing=OnRep_PowerupActive)
	bool bIsPowerupActive;//boolean to show whether or not the user has powerup or not

	UFUNCTION()
		void OnRep_PowerupActive();//when powerup is active function happens every tick

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnPowerupStateChange(bool bNewIsActive);//happens when powerup state changes

	AActor* TargetActor;

public:
	
	void ActivatePowerup(AActor* OtherActor);//function that activates powerup for given actor

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;//gets the lifetime

	
	virtual void OnPowerupTicked(AActor* OtherActor);//function when powerup is active

	virtual void OnActivated(AActor* OtherActor);//on activated 

	virtual void OnExpired(AActor* OtherActor);//on expired
};
