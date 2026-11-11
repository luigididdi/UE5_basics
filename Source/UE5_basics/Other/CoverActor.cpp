// Fill out your copyright notice in the Description page of Project Settings.


#include "CoverActor.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ACoverActor::ACoverActor() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Init. components
	SM = CreateDefaultSubobject<UStaticMeshComponent>(FName("SM"));
	BoxComp = CreateDefaultSubobject<UBoxComponent>(FName("BoxComp"));

	SetRootComponent(SM);

	BoxComp->SetupAttachment(SM);
}

void ACoverActor::BeginPlay() {
	Super::BeginPlay();

}

void ACoverActor::DetermineMovementDirection(const ACharacter* PlayerCharacter, FVector& MovementDirection, FRotator& FacingRotation) const
{
	
	if (!PlayerCharacter) return;

	const FVector PlayerLocation = PlayerCharacter->GetActorLocation();
    
	// Line trace dal Player verso il centro del CoverActor per rilevare la superficie
	const FVector TraceStart = PlayerLocation;
	const FVector TraceEnd = GetActorLocation();

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(PlayerCharacter);

	// Tracciamo una linea per trovare l'impatto sulla mesh/box del CoverActor
	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	FVector HitNormal = FVector::ZeroVector;

	if (bHit && HitResult.GetActor() == this) {
		HitNormal = HitResult.ImpactNormal;
	} else {
		// Fallback in caso il raycast fallisca: calcola la direzione approssimata dal centro della cover al player
		HitNormal = (PlayerLocation - GetActorLocation()).GetSafeNormal2D();
	}

	// Assicuriamoci che la normale sia perfettamente orizzontale sul piano XY
	HitNormal.Z = 0.0f;
	HitNormal.Normalize();

	// 1. FacingRotation: il personaggio deve guardare verso la parete (opposto della normale)
	FacingRotation = (-HitNormal).Rotation();

	// 2. MovementDirection: Vettore di scorrimento laterale lungo la parete
	// Il prodotto vettoriale tra la normale della parete e il vettore UP (0,0,1) genera l'asse di scorrimento lungo il muro
	MovementDirection = FVector::CrossProduct(HitNormal, FVector::UpVector).GetSafeNormal();
}


void ACoverActor::RetrieveMovementDirectionAndFacingRotation(ACharacter* PlayerCharacter, FVector& MovementDirection, FRotator& FacingRotation) const
{
	DetermineMovementDirection(PlayerCharacter,MovementDirection, FacingRotation);
}



