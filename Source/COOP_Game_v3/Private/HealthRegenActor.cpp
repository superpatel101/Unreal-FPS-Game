//HealthRegenActor.cpp
//2019-6-14
//This deals with the logic of the health powerup
//James Xu, Feeda Elahresh, Aaryan Patel

#include "HealthRegenActor.h"
#include "Public/SCharacter.h"
#include "Public/HealthComponent.h"

AHealthRegenActor::AHealthRegenActor()//constructor
{
	regenPerTick = 5;//every frame 5 health will be added
}


void AHealthRegenActor::OnPowerupTicked(AActor* OtherActor)//when its on the powerup
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
