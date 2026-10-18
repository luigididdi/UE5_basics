// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyHealthComponent.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealtDelegate);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE5_BASICS_API UMyHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyHealthComponent();
	
	UFUNCTION(BlueprintCallable)
	void GetDamage(int Damage);
	
	UFUNCTION(BlueprintCallable)
	void Healing(int Amount);
	
	UFUNCTION(BlueprintCallable)
	void ActivateHealing(bool Activate);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	int currentHealth;
	bool isHealing = false;
	
private:
	
	const int maxHealth = 100;
		
};

// TODO: Che abbia anche l'autorecover a tempo, tramite comando e due oggetti, uno che guarisce ed uno che fa danno
// TODO tututu nanana