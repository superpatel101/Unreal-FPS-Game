// Fill out your copyright notice in the Description page of Project Settings.

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
public:
	void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Behavior")
		class UBehaviorTree* BotBehavior;
};