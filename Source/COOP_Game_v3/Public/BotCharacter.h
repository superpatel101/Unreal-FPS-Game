//BotCharacter.h
//2019-6-14
//Header file for the character class concerning the bot
//James Xu, Feeda Elahresh, Aaryan Patel
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
    
    float TimeLastFired;//keeps track of the time when it last fired

	uint32 GetDecision();//gets the decision the bot makes

	void MoveTowardsEnemy(float DeltaTime, AActor* Target = nullptr);//function that makes bot move towards enemy

	void GoToHealthRegen(float DeltaTime);//function that makes bot go to health pack when health is low

	void GoToAmmoRefill(float DeltaTime);

	void Attack();//function that deals with the bot attacking

	bool bShouldFire;//boolean to check when the bot should or shouldn't be firing
	bool bShouldStopFire;
	
public:
	void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Behavior")
		class UBehaviorTree* BotBehavior;//the behaviour tree will set/organize priorities for the bot so decisions are made 
	uint32 AMMO_REFILL = 1;
	uint32 MOVE_TOWARDS_ENEMY = 2;
	uint32 RETREAT = 3;

	float MIN_DIST = 300;

	AActor* GetNearestOfClass(UClass* Type = ASCharacter::StaticClass());

};
