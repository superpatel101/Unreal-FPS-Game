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
	float IntervalBetweenTicks;

	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 TotalNumberOfTicks;

	//Total # of ticks applied
	int32 TicksCompleted;

	FTimerHandle TimerHandle_PowerupTick;

	UFUNCTION()
	void OnTickPowerup();//

	UPROPERTY(ReplicatedUsing=OnRep_PowerupActive)
	bool bIsPowerupActive;//boolean to show whether or not the user has powerup or not

	UFUNCTION()
		void OnRep_PowerupActive();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnPowerupStateChange(bool bNewIsActive);

	AActor* TargetActor;

public:
	
	void ActivatePowerup(AActor* OtherActor);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	// UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	virtual void OnPowerupTicked(AActor* OtherActor);

	// UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	virtual void OnActivated(AActor* OtherActor);

	// UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	virtual void OnExpired(AActor* OtherActor);
};
