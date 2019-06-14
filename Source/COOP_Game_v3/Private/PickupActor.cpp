//PickupActor.cpp
//2019-6-14
//This class deals with picking up powerups
//James Xu, Feeda Elahresh, Aaryan Patel

#include "Public/PickupActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "TimerManager.h"
// Sets default values
APickupActor::APickupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));//creates a sphere for where the pickup is
	SphereComp->SetSphereRadius(75.0f);
	RootComponent = SphereComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));//design attachment and creation
	DecalComp->SetupAttachment(RootComponent);
	DecalComp->SetRelativeRotation(FRotator(90, 0.f, 0.f));
	DecalComp->DecalSize = FVector(64, 75, 75);
	SetReplicates(true);
}

// Called when the game starts or when spawned
void APickupActor::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority)
	{
		Respawn();
	}
}

void APickupActor::Respawn()//respawns pickup after a while
{
	if (PowerUpClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("PowerupClass is nullptr!"), *GetName());
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	PowerupInstance = GetWorld()->SpawnActor<APowerupActor>(PowerUpClass, GetTransform(), Params);
}

void APickupActor::NotifyActorBeginOverlap(AActor* OtherActor)//notifies when actor is taken
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (Role == ROLE_Authority && PowerupInstance)
	{
		PowerupInstance->ActivatePowerup(OtherActor);//activated 
		PowerupInstance = nullptr;//set to null because its used

		GetWorldTimerManager().SetTimer(RespawnTimer, this, &APickupActor::Respawn, CoolDownDuration);//time delay until powerup respawns
	}
}

