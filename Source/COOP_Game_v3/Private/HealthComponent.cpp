//HealthComponent.cpp
//2019-6-14
//This class deals with the health component of players
//James Xu, Feeda Elahresh, Aaryan Patel

#include "HealthComponent.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "SCharacter.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	MaxHealth = 100;

	SetIsReplicated(true);

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)//if it's on server
	{

		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);//adds dynamic to take damage
		}
	}
	Health = MaxHealth;//sets health to max at start
}


void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{//allows for damage to be handled
	if (Damage <= 0.0f)//if there is damage
	{
		return;
	}
    ASCharacter* DamagedPlayer = Cast<ASCharacter>(DamagedActor);//gets both players that attacked and got hurt
    ASCharacter* DamageCauserPlayer = Cast<ASCharacter>(DamageCauser->GetOwner());

    if (DamagedPlayer)//if there is a player that got damaged
    {
        if (DamageCauserPlayer) {//and a player that damaged
            if (DamagedPlayer->TeamNum == DamageCauserPlayer->TeamNum) {//team members can't hurt each other
                return;
            }
        }
    }

	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);//gets the health after damage

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);//sets it and updates
}

float UHealthComponent::GetHealth()//getter for health
{
	return Health;
}

void UHealthComponent::Heal(float amount)//heals
{
	if (amount > 0.f && Health > 0.f)
	{
		Health = FMath::Clamp(Health + amount, 0.0f, MaxHealth);//updates health by adding an amount to it
	}
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHealthComponent, Health);
}

