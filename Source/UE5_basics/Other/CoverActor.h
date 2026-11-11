// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "CoverActor.generated.h"

UCLASS()
class UE5_BASICS_API ACoverActor : public AActor
{
	GENERATED_BODY()
	
private:

	/** Determines the movement direction and the facing direction of the player */
	void DetermineMovementDirection(const ACharacter* PlayerCharacter, FVector& MovementDirection, FRotator& FacingDirection) const;
	
protected:

	/** The box component that informs the player if he's able to take cover or not */
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComp;

public:
	// Sets default values for this actor's properties
	ACoverActor();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offset")
	FVector BoxCompOffset = FVector(50.0f, 50.0f, 0.0f);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* SM;

	/** Retrieves the movement direction and the facing rotation of the player */
	void RetrieveMovementDirectionAndFacingRotation(ACharacter* PlayerCharacter, FVector& MovementDirection, FRotator& FacingRotation) const;

};
