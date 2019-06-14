//BotCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "SCharacter.h"
#include "BotCharacter.generated.h"

/**
 * 
 */
UCLASS()
class COOP_GAME_V3_API ABotCharacter : public ASCharacter
{
	GENERATED_BODY()
protected:
    virtual void BeginPlay() override;
    
    UFUNCTION(Server, Reliable, WithValidation)
    virtual void ServerBeginPlay();
    
    float TimeLastFired;

	uint32 GetDecision();

	void MoveTowardsEnemy(float DeltaTime, AActor* Target = nullptr);

	void GoToHealthRegen(float DeltaTime);

	void GoToAmmoRefill(float DeltaTime);

	void Attack();

	bool bShouldFire;
	bool bShouldStopFire;
	
public:
	void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Behavior")
		class UBehaviorTree* BotBehavior;
	uint32 AMMO_REFILL = 1;
	uint32 MOVE_TOWARDS_ENEMY = 2;
	uint32 RETREAT = 3;

	float MIN_DIST = 300;

	AActor* GetNearestOfClass(UClass* Type = ASCharacter::StaticClass());

};
