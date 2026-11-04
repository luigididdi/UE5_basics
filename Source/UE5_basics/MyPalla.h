#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPalla.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class UE5_BASICS_API AMyPalla : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyPalla();

protected:
	virtual void BeginPlay() override;

	// Sfera di trigger per l'interazione
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USphereComponent> CollisionComp;

	// Mesh visiva della palla
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> MeshComp;

public:	
	virtual void Tick(float DeltaTime) override;

	// Funzioni di interazione
	void PickUp(AActor* TargetCharacter);
	void Drop();
	void Throw();

	// Funzioni di Overlap (richiedono UFUNCTION e la firma completa)
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};