//HealthRegenActor.cpp

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
			PlayerHealthComp->Heal(regenPerTick);
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
