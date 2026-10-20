// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthZone.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class UE5_BASICS_API AHealthZone : public AActor
{
	GENERATED_BODY()

public:
	AHealthZone();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Trigger;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	/** true = cura, false = danneggia */
	UPROPERTY(EditAnywhere, Category="Health")
	bool bHeals = false;

	UPROPERTY(EditAnywhere, Category="Health")
	int32 Amount = 20;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
