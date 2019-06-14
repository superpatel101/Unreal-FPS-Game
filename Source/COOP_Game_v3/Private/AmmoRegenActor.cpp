//AmmoRegenActor.cpp

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
