//AmmoRegenActor.cpp
//2019-6-14
//Deals with the logic of the ammo pack
//James Xu, Feeda Elahresh, Aaryan Patel

#include "AmmoRegenActor.h"
#include "SCharacter.h"

void AAmmoRegenActor::OnActivated(AActor* OtherActor)
{
	Super::OnActivated(OtherActor);
	ASCharacter* OwningChar = Cast<ASCharacter>(OtherActor);

	if (OwningChar)
	{
		OwningChar->AddAmmo(AmmoAmount);
	}
}
