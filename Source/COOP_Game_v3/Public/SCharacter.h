// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Public/HealthComponent.h"


#include "SCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;

class ASWeapon;

class UHealthComponent;

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
	
	void MoveForward(float Value);
	
	void MoveRight(float Value);

	void BeginCrouch();

	void EndCrouch();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	bool bWantsToZoom;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly,Category = "Player", meta = (ClampMin = 0.1,ClampMax = 100))
	float ZoomInterpSpeed;
	
	float DefaultFOV;

	void BeginZoom();
	void EndZoom();

	UPROPERTY(Replicated)
	ASWeapon* CurrentWeapon;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Player")
	TSubclassOf<ASWeapon> StarterWeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName WeaponAttachSocketName;

	void StartFire();

	void StopFire();

	void Reload();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerReload();

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 LoadedAmmo;
	
	UPROPERTY(Replicated, EditAnywhere,BlueprintReadWrite, Category = "Ammo")
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
};
