// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lezione2/TP_Character/TP_TestCharacter.h"
#include "MyPalla.generated.h"

UCLASS()
class UE5_BASICS_API AMyPalla : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyPalla();
	
	//ATP_TestCharacter Character;
	
	/*//DA UN FORUM!
	UPROPERTY(EditAnywhere, Category="Collision")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	bool canBePicked = false;
	void IsHit(bool isHit);
	void Picked();
};
