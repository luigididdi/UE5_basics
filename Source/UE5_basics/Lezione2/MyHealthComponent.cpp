// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHealthComponent.h"

// Sets default values for this component's properties
UMyHealthComponent::UMyHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	LogHealth(TEXT("Start"));
}

void UMyHealthComponent::GetDamage(int32 Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0, MaxHealth);
	LogHealth(FString::Printf(TEXT("Danno -%d"), Damage));
}

void UMyHealthComponent::Healing(int32 Amount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0, MaxHealth);
	LogHealth(FString::Printf(TEXT("Cura +%d"), Amount));
}

void UMyHealthComponent::ActivateHealing(bool Activate)
{
	bIsHealing = Activate;
	TimeSinceLastHeal = 0.f;
	LogHealth(Activate ? TEXT("Autocura ON") : TEXT("Autocura OFF"));
}

// Called every frame
void UMyHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsHealing) return;

	TimeSinceLastHeal += DeltaTime;
	if (TimeSinceLastHeal >= AutoHealInterval)
	{
		TimeSinceLastHeal = 0.f;
		Healing(AutoHealAmount);
	}
}

void UMyHealthComponent::LogHealth(const FString& Reason) const
{
	const FString Msg = FString::Printf(TEXT("%s | %s -> HP %d/%d"),
		*GetOwner()->GetName(), *Reason, CurrentHealth, MaxHealth);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);               // finestra Output Log
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, Msg); // a schermo (key 1 = sovrascrive il messaggio precedente)
	}
}

