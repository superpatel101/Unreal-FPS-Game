//AmmoRegenActor.cpp
//2019-6-14
//Deals with the logic of the ammo pack
//James Xu, Feeda Elahresh, Aaryan Patel

#include "AmmoRegenActor.h"
#include "SCharacter.h"

void AAmmoRegenActor::OnActivated(AActor* OtherActor)//when activated
{
	Super::OnActivated(OtherActor);
	ASCharacter* OwningChar = Cast<ASCharacter>(OtherActor);

	if (OwningChar)//if there is a character that went over the pack
	{
		OwningChar->AddAmmo(AmmoAmount);//we simply add ammo
	}
}
