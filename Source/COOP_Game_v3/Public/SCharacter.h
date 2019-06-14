//SCharacter.h
//This is the header file for the SCharacter class, it initializes all variables, functions, and classes concerning the .cpp class of SCharacter

#pragma once

#include "CoreMinimal.h"//all the imports
#include "GameFramework/Character.h"
#include "Public/HealthComponent.h"
#include "CTF_Flag.h"

#include "SCharacter.generated.h"


class UCameraComponent;//classes used
class USpringArmComponent;
class ASWeapon;
class UHealthComponent;
class ACTF_Flag;

UCLASS()
class COOP_GAME_V3_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);//movement functions
	void MoveRight(float Value);
	void BeginCrouch();
	void EndCrouch();

    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")//a field unreal will use to assign a camera component
		UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")//spring arm component will also be used by unreal and will be used in junction with camera component for better third person viewing
		USpringArmComponent* SpringArmComp;


	void BeginZoom();//functions concerning the player zooming in and out when aiming
	void EndZoom();
	float DefaultFOV;//this float holds the default field of vision
	UPROPERTY(EditDefaultsOnly, Category = "Player")
		float ZoomedFOV;//will hold the zoomed field of vision
	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
		float ZoomInterpSpeed;//will hold how long it'll take to zoom
	bool bWantsToZoom;//boolean to keep track of whether the user is trying to zoom or not

	
	UPROPERTY(Replicated)
		ASWeapon* CurrentWeapon;//weapon object field

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		TSubclassOf<ASWeapon> StarterWeaponClass;//will take in a blueprint class on the engine side (primary)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")//same as above but for secondary weapon
		TSubclassOf<ASWeapon> SecondaryWeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")//this holds the name of the socket where the weapon will attach to (socket on skeletal mesh of character)
		FName WeaponAttachSocketName;

	void StartFire();

	void StopFire();

	void Reload();

	void SwitchWeapon();

	int32 Primary_LoadedAmmo;
	int32 Primary_AmmoPool;
	int32 Secondary_LoadedAmmo;
	int32 Secondary_AmmoPool;

	float Primary_ZoomedFOV;
	float Secondary_ZoomedFOV;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSwitchWeapon();

	UPROPERTY(Replicated)
		int32 LoadedAmmosEach[2];

	UPROPERTY(Replicated)
		int32 AmmoPoolsEach[2];

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		int32 OnMainWeapon;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerReload();

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int32 LoadedAmmo;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int32 AmmoPool;

	UHealthComponent* HealthComponent;

	UFUNCTION()
		void OnHealthChanged(UHealthComponent* HealthComponent, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
		bool bDied; //Died previously
    


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerReduceAmmoByOne();

	void ReduceAmmoByOne();

	int32 GetLoadedAmmo();

	void AddAmmo(int32 Amount);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddAmmo(int32 Amount);

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Health Component")
		int32 TeamNum;
    
    UFUNCTION(BlueprintCallable)
    FString GetTeamName();

	bool IsFriendly(ASCharacter* A, ASCharacter* B)
	{
		if (A == nullptr || B == nullptr)
		{
			return true; // assume friendly
		}
		return A->TeamNum == B->TeamNum;
	}

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
		FName FlagAttachSocketName;

	ACTF_Flag* CurrentFlag;
};
