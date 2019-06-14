//HealthComponent.h
//2019-6-14
//Header file concerning the health component of the actors. Keeps track of health, damage, and any healing
//James Xu, Feeda Elahresh, Aaryan Patel
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

//On health changed
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, UHealthComponent*, HealthComponent, float, Health, float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);


UCLASS( ClassGroup=(COOP), meta=(BlueprintSpawnableComponent) )
class COOP_GAME_V3_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "HealthComponent")
	float Health;//health value for whichever actor

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthComponent")
	float MaxHealth;//maximum health to make sure when healed it doesn't have too much health

	UFUNCTION()//function that handles damage
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//function that gets the liftime props
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnHealthChangedSignature OnHealthChanged;//value that keeps track of whether the health is changed or not
	float GetHealth();//getter

	void Heal(float amount);//adds health
};
