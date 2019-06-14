//CTF_Flag.cpp
//2019-6-14
//Deals with the logic of the flag (suitcase)
//James Xu, Feeda Elahresh, Aaryan Patel

#include "CTF_Flag.h"
#include "SCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HealthComponent.h"
#include "CTFPickup.h"
#include "Kismet/GameplayStatics.h"
#include "CTF_GameStateBase.h"
#include "CTF_GameModeBase.h"
#include "COOP_Game_v3.h"
#include "Engine/Engine.h"
#include "UnrealNetwork.h"
#include "DrawDebugHelpers.h"


// Sets default values
ACTF_Flag::ACTF_Flag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ACTF_Flag::BeginPlay()//starts the game mode
{
	Super::BeginPlay();
	if (Role == ROLE_Authority)
	{
		TArray<AActor*> FlagPickupLocations;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACTFPickup::StaticClass(), FlagPickupLocations);
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("START"));
		

		for (int i = 0; i < FlagPickupLocations.Num(); i++)
		{
			ACTFPickup* Loc = Cast<ACTFPickup>(FlagPickupLocations[i]);
			if (Loc)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::SanitizeFloat(Loc->GetDistanceTo(this)));

				if (Loc->GetDistanceTo(this) > 0.0f)//(Loc->GetTeamNum() != TeamNum)
				{
					Destination = Loc;
					//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::FromInt(TeamNum));
					//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::FromInt(Loc->GetTeamNum()));

				}
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("END"));

	} else
	{
		ServerBeginPlay();
	}
}

void ACTF_Flag::ServerBeginPlay_Implementation()
{
	BeginPlay();
}

bool ACTF_Flag::ServerBeginPlay_Validate()
{
	return true;
}

void ACTF_Flag::OnReachDest()
{
	if (Role == ROLE_Authority)
	{
		Detach();

		ACTF_GameModeBase* GM = Cast<ACTF_GameModeBase>(GetWorld()->GetAuthGameMode());

		if (GM)
		{
			if (TeamNum == TEAM_RED)
			{
				GM->FlagCapture(TEAM_BLUE);
			} else
			{
				GM->FlagCapture(TEAM_RED);
			}
		}
	}
}

// Called every frame
void ACTF_Flag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Role == ROLE_Authority)
	{
		ASCharacter* Owner = Cast<ASCharacter>(GetOwner());
		if (Owner)
		{
			if (Destination->GetDistanceTo(Owner) <= 200.f)
			{
				OnReachDest();
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("dest reached"));
				DrawDebugSphere(
					GetWorld(),
					Destination->GetActorLocation(),
					240,
					32,
					FColor(255, 0, 0)
				);
			}
			UHealthComponent* OwnerHealth = Cast<UHealthComponent>(Owner->GetComponentByClass(UHealthComponent::StaticClass()));
			if (ensure(OwnerHealth))
			{
				if (OwnerHealth->GetHealth() == 0)
				{
					Detach();
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("no health"));

				}
			}
		}
	}

}

void ACTF_Flag::AttachFlagToPlayer(ASCharacter* Player)
{
	if (Role == ROLE_Authority)
	{
		if (Player)
		{
			FName Socket = Player->FlagAttachSocketName;
			this->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Socket);
			SetOwner(Player);
		}
	} else
	{
		ServerAttachFlagToPlayer(Player);
	}
}

void ACTF_Flag::ServerAttachFlagToPlayer_Implementation(ASCharacter* Player)
{
	AttachFlagToPlayer(Player);
}

bool ACTF_Flag::ServerAttachFlagToPlayer_Validate(ASCharacter* Player)
{
	return true;
}



void ACTF_Flag::Detach()
{
	if (Role == ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("Detached..."));
		TArray<AActor*> FlagPickupLocations;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACTFPickup::StaticClass(), FlagPickupLocations);

		for (int i = 0; i < FlagPickupLocations.Num(); i++)
		{
			ACTFPickup* Loc = Cast<ACTFPickup>(FlagPickupLocations[i]);
			if (Loc)
			{
				if (Loc->GetTeamNum() == TeamNum)
				{
					Destroy();
					DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
					Loc->Respawn();
					return;
				}
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("couldn't find respawn loc"));

	} else
	{
		ServerDetach();
	}
}

void ACTF_Flag::ServerDetach_Implementation()
{
	Detach();
}

bool ACTF_Flag::ServerDetach_Validate()
{
	return true;
}

void ACTF_Flag::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACTF_Flag, TeamNum);
}
