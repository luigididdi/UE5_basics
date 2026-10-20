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
	
	UFUNCTION(BlueprintCallable, Category="Health")
	void GetDamage(int Damage);
	
	UFUNCTION(BlueprintCallable, Category="Health")
	void Healing(int Amount);
	
	/** Attiva/disattiva la cura automatica */
	UFUNCTION(BlueprintCallable, Category="Health")
	void ActivateHealing(bool Activate);
	
	UFUNCTION(BlueprintCallable, Category="Health")
	void ToggleAutoHeal() { ActivateHealing(!bIsHealing); }

	UFUNCTION(BlueprintPure, Category="Health")
	int32 GetCurrentHealth() const { return CurrentHealth; }

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Health")
	int32 MaxHealth = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
	int32 CurrentHealth = 0;

	/** Quanti HP recupera l'autocura a ogni "tick" di cura */
	UPROPERTY(EditAnywhere, Category="Health|AutoHeal")
	int32 AutoHealAmount = 5;

	/** Ogni quanti secondi scatta l'autocura */
	UPROPERTY(EditAnywhere, Category="Health|AutoHeal")
	float AutoHealInterval = 1.0f;
	
private:
	
	bool bIsHealing = false;
	float TimeSinceLastHeal = 0.f;

	void LogHealth(const FString& Reason) const;
		
};

// TODO: Che abbia anche l'autorecover a tempo, tramite comando e due oggetti, uno che guarisce ed uno che fa danno