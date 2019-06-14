//CTFPickup.cpp

#include "CTFPickup.h"
#include "CTF_Flag.h"
#include "SCharacter.h"
#include "UnrealNetwork.h"
#include "Engine/Engine.h"

// Sets default values
ACTFPickup::ACTFPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(75.0f);
	RootComponent = SphereComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetupAttachment(RootComponent);
	DecalComp->SetRelativeRotation(FRotator(90, 0.f, 0.f));
	DecalComp->DecalSize = FVector(64, 75, 75);
	SetReplicates(true);

}

// Called when the game starts or when spawned
void ACTFPickup::BeginPlay()
{
	Super::BeginPlay();
	SetReplicates(true);
	Respawn();
	

}

void ACTFPickup::Respawn()
{
	if (Role == ROLE_Authority)
	{
		if (PowerUpClass == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("PowerupClass is nullptr!"), *GetName());
			return;
		}
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FlagInstance = GetWorld()->SpawnActor<ACTF_Flag>(PowerUpClass, GetTransform(), Params);
		FlagInstance->TeamNum = TeamNum;
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::FromInt(TeamNum));
	} else
	{
		ServerRespawn();
	}
}

void ACTFPickup::ServerRespawn_Implementation()
{
	Respawn();
}

bool ACTFPickup::ServerRespawn_Validate()
{
	return true;
}

void ACTFPickup::GiveFlag(ASCharacter* Target)
{
	if (FlagInstance)
	{
		FlagInstance->AttachFlagToPlayer(Target);
	}
}

void ACTFPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	ASCharacter* OverlapChar = Cast<ASCharacter>(OtherActor);
	if (OverlapChar)
	{
		if (OverlapChar->TeamNum != TeamNum)
		{
			GiveFlag(OverlapChar);
		}
	}
}

void ACTFPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACTFPickup, FlagInstance);
}