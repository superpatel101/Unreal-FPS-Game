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

	if (GetOwnerRole() == ROLE_Authority)
	{

		AActor* MyOwner = GetOwner();
		if (MyOwner)
		{
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
		}
	}
	Health = MaxHealth;
}


void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}
    ASCharacter* DamagedPlayer = Cast<ASCharacter>(DamagedActor);
    ASCharacter* DamageCauserPlayer = Cast<ASCharacter>(DamageCauser->GetOwner());

    if (DamagedPlayer)
    {
        if (DamageCauserPlayer) {
            if (DamagedPlayer->TeamNum == DamageCauserPlayer->TeamNum) {
                return;
            }
        }
    }

	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));

	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}

float UHealthComponent::GetHealth()
{
	return Health;
}

void UHealthComponent::Heal(float amount)
{
	if (amount > 0.f && Health > 0.f)
	{
		Health = FMath::Clamp(Health + amount, 0.0f, MaxHealth);
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

