// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthZone.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UE5_basics/Lezione2/MyHealthComponent.h"

AHealthZone::AHealthZone()
{
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	SetRootComponent(Trigger);
	Trigger->InitSphereRadius(100.f);
	Trigger->SetCollisionProfileName(TEXT("Trigger"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Trigger);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // la mesh è solo visiva
}

void AHealthZone::BeginPlay()
{
	Super::BeginPlay();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AHealthZone::OnOverlapBegin);
}

void AHealthZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	if (UMyHealthComponent* HP = OtherActor->FindComponentByClass<UMyHealthComponent>())
	{
		if (bHeals) HP->Healing(Amount);
		else        HP->GetDamage(Amount);
	}
}

