//SCharacter.cpp
//2019-6-14
//This is where the actual logic of the character is coded
//James Xu, Feeda Elahresh, Aaryan Patel
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

#include "CTF_PlayerState.h"

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

	Primary_LoadedAmmo=30;//this is the mag ammo default for rifle
	Primary_AmmoPool=30;//ammo pool default for rifle
	Secondary_LoadedAmmo = 10;//same as above but for the grenade launcher
	Secondary_AmmoPool = 10;

	LoadedAmmo = Primary_LoadedAmmo;//by default the user is using the rifle so that's what the main variables will hold
	AmmoPool = Primary_AmmoPool;
	
	LoadedAmmosEach[0] = Primary_LoadedAmmo;//sets the list values for loadedAmmosEach
	LoadedAmmosEach[1] = Secondary_LoadedAmmo;

	AmmoPoolsEach[0] = Primary_AmmoPool;//same as above but for AmmoPoolsEach
	AmmoPoolsEach[1] = Secondary_AmmoPool;

	Primary_ZoomedFOV = 65.0f;//zoom amount for rifle
	Secondary_ZoomedFOV = 90.0f;//zoome amount for grenade launcher
	ZoomedFOV = Primary_ZoomedFOV;//default rifle
	ZoomInterpSpeed = 20;//zoom speed
	OnMainWeapon = 1;//checks if its on main weapon or not

	WeaponAttachSocketName = "WeaponSocket";//character socket on right hand where weapon will attach

	FlagAttachSocketName = "FlagSocket";//socket where flag will attach
	



	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));//health component object


}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	DefaultFOV = CameraComp->FieldOfView;//gets the FOV
	HealthComponent->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
	if (Role == ROLE_Authority)
	{
	
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (OnMainWeapon == 1) {//if its supposed to be on rifle
			CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			//current weapon becomes the rifle which is the StarterWeaponClass in blueprints
		}
		else {//same as above but for grenade launcher
			CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(SecondaryWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		}
		
		if (CurrentWeapon) {//if their is a current weapon we set the owner and attach to mesh

			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		}

	} else
    {
        UE_LOG(LogTemp, Warning, TEXT("Weapon spawn failed: not ROLE_Authority"));
    }
}

//movement input
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
	Crouch();//these are inbuilt functions
}
void ASCharacter::EndCrouch()
{
	UnCrouch();
}

void ASCharacter::BeginZoom()
{
	bWantsToZoom = true;//keeps track of whether the character is trying to zoom or not
}

void ASCharacter::EndZoom()
{
	bWantsToZoom = false;
}

void ASCharacter::StartFire()
{
	
	if (CurrentWeapon) {
		if (LoadedAmmo <= 0) {//if no ammo StartFire doesn't happen
			return;
		}
		CurrentWeapon->StartFire();//otherwise we take the weapon object and do its startfire function 
	}
}
void ASCharacter::StopFire()//same idea as above 
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
void ASCharacter::SwitchWeapon()//this function deals with switching weapons
{
	if (Role == ROLE_Authority)//if on server
	{
		FActorSpawnParameters SpawnParams;//in built object type
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//this just overrides collisions to make sure the next thing spawns
		CurrentWeapon->Destroy();//destroys the current mesh
		if (OnMainWeapon == 1) {//if on rifle
			OnMainWeapon = 2;//it becomes grenade launcher
			LoadedAmmosEach[0] = LoadedAmmo;//updates list with current loaded ammo and ammo pool
			AmmoPoolsEach[0] = AmmoPool;
			LoadedAmmo = LoadedAmmosEach[1];//updates the loaded ammo and ammo pool to the grenade launcher one
			AmmoPool = AmmoPoolsEach[1];
			ZoomedFOV = Secondary_ZoomedFOV;//updates zoomed fov to the fov value when grenade launcher is equipped
			CurrentWeapon->SetFireRate(600);//sets the fire rate
		}
		else {//if on grenade launcher
			OnMainWeapon = 1;//becomes rifle
			LoadedAmmosEach[1] = LoadedAmmo;//updates the grenade launcher values in the list to be current
			AmmoPoolsEach[1] = AmmoPool;

			LoadedAmmo = LoadedAmmosEach[0];//becomes the rifle values
			AmmoPool = AmmoPoolsEach[0];
			ZoomedFOV = Primary_ZoomedFOV;
			CurrentWeapon->SetFireRate(600);
		}
		if (OnMainWeapon == 1) {//updates current weapon for rifle
			CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		}
		else {//updates current weapon to grenade launcher
			CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(SecondaryWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		}
		if (CurrentWeapon) {//once current weapon has been defined its owner is set and its attached to the mesh

			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		}
	} else//if it doesn't have server authority it runs the code on the server
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

	ACTF_PlayerState* PlayerState = Cast<ACTF_PlayerState>(GetPlayerState());
	if (PlayerState)
	{
		TeamNum = PlayerState->TeamNum;
		// UE_LOG(LogTemp, Warning, TEXT("SCharacter Team: %d"), TeamNum);
	}
	else
	{
		// UE_LOG(LogTemp, Error, TEXT("PLAYER STATE IS NULL!!"))
	}
	 

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)//this just sets up input and connects it with our functions
{
	//all these actions are bound to certain keys in unreal
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);//basic movement
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::BeginCrouch);//various actions
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
	if (CameraComp) {//if there is a camera component set up
		return CameraComp->GetComponentLocation();//we just get the location
	}
	return Super::GetPawnViewLocation();//otherwise we return the player pawn's location
	
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

void ASCharacter::ReduceAmmoByOne()//reduces ammo when necessary
{
	if (Role == ROLE_Authority)//client side
	{
		LoadedAmmo--;
	}
	else//server side
	{
		ServerReduceAmmoByOne();
	}
}

void ASCharacter::ServerReduceAmmoByOne_Implementation()//reduces ammo on server side
{
	ReduceAmmoByOne();
}

bool ASCharacter::ServerReduceAmmoByOne_Validate()
{
	return true;
}


void ASCharacter::AddAmmo(int32 Amount)//adds ammo
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



int32 ASCharacter::GetLoadedAmmo()//getter for loaded ammo
{
	return LoadedAmmo;
}


FString ASCharacter::GetTeamName() {//getter for the team name
    if (TeamNum == TEAM_RED) {
        return FString(TEXT("Red Team"));
    } else if (TeamNum == TEAM_BLUE) {
        return FString(TEXT("Blue Team"));
    } else
    {
		return FString(TEXT("INVALID TEAM"));
    }
}
