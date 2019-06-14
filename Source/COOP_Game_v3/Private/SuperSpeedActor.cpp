//SuperSpeedActor.cpp
//2019-6-14
//This deals with the logic when a player picks up a super speed powerup
//James Xu, Feeda Elahresh, Aaryan Patel
#include "SuperSpeedActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Public/SCharacter.h"
#include "Engine/Engine.h"

ASuperSpeedActor::ASuperSpeedActor()
{
	speedMultiplier = 2.0f;
}

void ASuperSpeedActor::OnActivated(AActor* OtherActor)
{
	Super::OnActivated(OtherActor);
	ASCharacter* OtherPlayer = Cast<ASCharacter>(OtherActor);

	if (OtherPlayer && Role == ROLE_Authority)
	{
		OtherPlayer->GetCharacterMovement()->MaxWalkSpeed *= speedMultiplier;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("should be faster...."));
	} else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Uhhh"));
		}
	}
	
}

void ASuperSpeedActor::OnExpired(AActor* OtherActor)
{
	Super::OnExpired(OtherActor);
	ASCharacter* OtherPlayer = Cast<ASCharacter>(OtherActor);

	if (OtherPlayer)
	{
		OtherPlayer->GetCharacterMovement()->MaxWalkSpeed /= speedMultiplier;
	}
}

void ASuperSpeedActor::OnPowerupTicked(AActor* OtherActor)
{
	Super::OnPowerupTicked(OtherActor);
	//OtherActor->SetActorHiddenInGame(true);
}
