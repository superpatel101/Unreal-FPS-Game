//SCharacter.cpp
//This is where the actual logic of the character is coded

#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"

#include "SWeapon.h"

#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Public/HealthComponent.h"
#include "Engine/Engine.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "UnrealNetwork.h"

#include "COOP_Game_v3.h"

#include "CTF_GameModeBase.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	// GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	Primary_LoadedAmmo=30;
	Primary_AmmoPool=30;
	Secondary_LoadedAmmo = 10;
	Secondary_AmmoPool = 10;

	LoadedAmmo = Primary_LoadedAmmo;
	AmmoPool = Primary_AmmoPool;
	
	LoadedAmmosEach[0] = Primary_LoadedAmmo;
	LoadedAmmosEach[1] = Secondary_LoadedAmmo;

	AmmoPoolsEach[0] = Primary_AmmoPool;
	AmmoPoolsEach[1] = Secondary_AmmoPool;
	Primary_ZoomedFOV = 65.0f;
	Secondary_ZoomedFOV = 90.0f;
	ZoomedFOV = Primary_ZoomedFOV;
	ZoomInterpSpeed = 20;
	OnMainWeapon = 1;

	WeaponAttachSocketName = "WeaponSocket";

	FlagAttachSocketName = "FlagSocket";
	



	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));


}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	DefaultFOV = CameraComp->FieldOfView;
	HealthComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
	if (Role == ROLE_Authority)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (OnMainWeapon == 1) {
			CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		}
		else {
			CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(SecondaryWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		}
		
		if (CurrentWeapon) {

			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		}
		// CurrentWeapon->SetFireRate(300);

	} else
    {
        UE_LOG(LogTemp, Warning, TEXT("Weapon spawn failed: not ROLE_Authority"));
    }
}

void ASCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector()*Value);
}

void ASCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void ASCharacter::BeginCrouch()
{
	Crouch();
}

void ASCharacter::EndCrouch()
{
	UnCrouch();
}

void ASCharacter::BeginZoom()
{
	bWantsToZoom = true;
}

void ASCharacter::EndZoom()
{
	bWantsToZoom = false;
}

void ASCharacter::StartFire()
{
	
	if (CurrentWeapon) {
		if (LoadedAmmo <= 0) {
			return;
		}
		CurrentWeapon->StartFire();
	}
}
void ASCharacter::StopFire()
{
	if (CurrentWeapon) {
		CurrentWeapon->StopFire();
	}
}
void ASCharacter::Reload()
{
	if (Role == ROLE_Authority)
	{
		if (AmmoPool <= 0 || LoadedAmmo >= 30) {
			return;
		}
		if (AmmoPool < (30 - LoadedAmmo)) {
			LoadedAmmo += AmmoPool;
			AmmoPool = 0;
		}
		else {
			AmmoPool = AmmoPool - (30 - LoadedAmmo);
			LoadedAmmo = 30;
		}
	} else
	{
		ServerReload();
	}
}
void ASCharacter::SwitchWeapon()
{
	if (Role == ROLE_Authority)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		CurrentWeapon->Destroy();
		if (OnMainWeapon == 1) {
			OnMainWeapon = 2;
			LoadedAmmosEach[0] = LoadedAmmo;
			AmmoPoolsEach[0] = AmmoPool;
			LoadedAmmo = LoadedAmmosEach[1];
			AmmoPool = AmmoPoolsEach[1];
			ZoomedFOV = Secondary_ZoomedFOV;
			CurrentWeapon->SetFireRate(600);
		}
		else {
			OnMainWeapon = 1;
			LoadedAmmosEach[1] = LoadedAmmo;
			AmmoPoolsEach[1] = AmmoPool;

			LoadedAmmo = LoadedAmmosEach[0];
			AmmoPool = AmmoPoolsEach[0];
			ZoomedFOV = Primary_ZoomedFOV;
			CurrentWeapon->SetFireRate(600);
		}
		if (OnMainWeapon == 1) {
			CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		}
		else {
			CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(SecondaryWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		}
		if (CurrentWeapon) {

			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		}
	} else
	{
		ServerSwitchWeapon();
	}
}

void ASCharacter::ServerSwitchWeapon_Implementation()
{
	SwitchWeapon();
}

bool ASCharacter::ServerSwitchWeapon_Validate()
{
	return true;
}



void ASCharacter::ServerReload_Implementation()
{
	Reload();
}

bool ASCharacter::ServerReload_Validate()
{
	return true;
}

void ASCharacter::OnHealthChanged(UHealthComponent* THealthComponent, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bDied)
	{
		// Die time
		bDied = true;

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();

		SetLifeSpan(1.0f);
        
        
        


	}
}




// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;

	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);
	// CurrentWeapon->SetFireRate(300);
	// float num = CurrentWeapon->GetFireRate();

	// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("%f"),num));
	CameraComp->SetFieldOfView(NewFOV);

	 

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::EndCrouch);
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ASCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ASCharacter::EndZoom);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASCharacter::StopFire);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ASCharacter::Reload);
	PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &ASCharacter::SwitchWeapon);

}

FVector ASCharacter::GetPawnViewLocation() const
{
	if (CameraComp) {
		return CameraComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
	
}

void ASCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCharacter, CurrentWeapon);
	DOREPLIFETIME(ASCharacter, bDied);
	DOREPLIFETIME(ASCharacter, LoadedAmmo);
	DOREPLIFETIME(ASCharacter, AmmoPool);
	DOREPLIFETIME(ASCharacter, LoadedAmmosEach);
	DOREPLIFETIME(ASCharacter, AmmoPoolsEach);
	DOREPLIFETIME(ASCharacter, TeamNum);
}

void ASCharacter::ReduceAmmoByOne()
{
	if (Role == ROLE_Authority)
	{
		LoadedAmmo--;
	}
	else
	{
		ServerReduceAmmoByOne();
	}
}

void ASCharacter::ServerReduceAmmoByOne_Implementation()
{
	ReduceAmmoByOne();
}

bool ASCharacter::ServerReduceAmmoByOne_Validate()
{
	return true;
}


void ASCharacter::AddAmmo(int32 Amount)
{
	if (Role == ROLE_Authority)
	{
		AmmoPool += Amount;
		UE_LOG(LogTemp, Warning, TEXT("Adding Ammo As Server"));
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::FromInt(AmmoPool));

	} else
	{
		ServerAddAmmo(Amount);
	}
}


void ASCharacter::ServerAddAmmo_Implementation(int32 Amount)
{
	AddAmmo(Amount);
}

bool ASCharacter::ServerAddAmmo_Validate(int32 Amount)
{
	return true;
}



int32 ASCharacter::GetLoadedAmmo()
{
	return LoadedAmmo;
}


FString ASCharacter::GetTeamName() {
    if (TeamNum == TEAM_RED) {
        return FString(TEXT("Red Team"));
    } else {
        return FString(TEXT("Blue Team"));
    }
}
