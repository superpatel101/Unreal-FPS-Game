// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthRegenActor.h"
#include "Public/SCharacter.h"
#include "Public/HealthComponent.h"

AHealthRegenActor::AHealthRegenActor()
{
	regenPerTick = 5;
}


void AHealthRegenActor::OnPowerupTicked(AActor* OtherActor)
{
	Super::OnPowerupTicked(OtherActor);

	ASCharacter* OtherPlayer = Cast<ASCharacter>(OtherActor);

	if (OtherPlayer)
	{
		UHealthComponent* PlayerHealthComp = Cast<UHealthComponent>(OtherPlayer->GetComponentByClass(UHealthComponent::StaticClass()));

		if (PlayerHealthComp)
		{
		
		}
	}
}

void AHealthRegenActor::OnActivated(AActor* OtherActor)
{
	Super::OnActivated(OtherActor);
}

void AHealthRegenActor::OnExpired(AActor* OtherActor)
{
	Super::OnPowerupTicked(OtherActor);
}
