#include "MyPalla.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "Lezione2/TP_Character/TP_TestCharacter.h"

AMyPalla::AMyPalla()
{
	PrimaryActorTick.bCanEverTick = false;

	// 1. La Mesh della palla diventa la Root e gestisce la fisica
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetSimulatePhysics(true); // Cade a terra e rotola con la gravità

	// 2. La sfera di collisione (Trigger) viene attaccata alla Mesh
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->SetupAttachment(RootComponent);
	CollisionComp->InitSphereRadius(80.f); // Aumentato leggermente per comodità di pickup
	CollisionComp->SetCollisionProfileName(TEXT("Trigger"));
}

void AMyPalla::BeginPlay()
{
	Super::BeginPlay();
    
	// Colleghiamo l'evento di overlap della sfera
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMyPalla::OnOverlapBegin);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AMyPalla::OnOverlapEnd);
}

void AMyPalla::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ATP_TestCharacter* Character = Cast<ATP_TestCharacter>(OtherActor))
	{
		Character->canPickBall = true;
		Character->CurrentBall = this;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, "Premi il tasto per raccogliere la palla!");
	}
}

void AMyPalla::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ATP_TestCharacter* Character = Cast<ATP_TestCharacter>(OtherActor))
	{
		if (!Character->doIHaveBall && Character->CurrentBall == this)
		{
			Character->canPickBall = false;
			Character->CurrentBall = nullptr;
		}
	}
}

void AMyPalla::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyPalla::PickUp(AActor* TargetCharacter)
{
	if (!TargetCharacter) return;

	// Disattiviamo fisica e collisioni a terra
	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); // <-- Ottimo, tieni questa linea!

	// Attacchiamo la palla mantenendo la trasformazione relativa
	/*FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelativeTransform, true);
	AttachToActor(TargetCharacter, AttachmentRules);*/
	// Passi direttamente la regola statica pronta all'uso
	AttachToActor(TargetCharacter, FAttachmentTransformRules::KeepRelativeTransform);

	// Posizioniamo la palla davanti al personaggio e leggermente sollevata
	SetActorRelativeLocation(FVector(100.f, 0.f, 50.f));
}

void AMyPalla::Drop()
{
	// Ricaviamo il personaggio a cui era attaccata prima di staccarla
	AActor* AttachedChar = GetAttachParentActor();

	// Stacchiamo la palla dal personaggio
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	if (AttachedChar)
	{
		// Calcoliamo la posizione 100 unità in avanti rispetto a dove guarda il player
		FVector DropLocation = AttachedChar->GetActorLocation() + AttachedChar->GetActorForwardVector() * 100.f + FVector(0.f, 0.f, 20.f);
		SetActorLocation(DropLocation);
	}

	// Riattiviamo collisioni e fisica per farla cadere a terra
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetSimulatePhysics(true);
}

void AMyPalla::Throw()
{
	// Ricaviamo il personaggio a cui era attaccata
	AActor* AttachedChar = GetAttachParentActor();

	// Stacchiamo la palla dal personaggio
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	if (AttachedChar)
	{
		// Posizioniamo la palla leggermente davanti al player prima di lanciare
		FVector ThrowLocation = AttachedChar->GetActorLocation() + AttachedChar->GetActorForwardVector() * 120.f + FVector(0.f, 0.f, 50.f);
		SetActorLocation(ThrowLocation);
	}

	// Riattiviamo collisioni e fisica per permettere alla palla di volare e rimbalzare
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetSimulatePhysics(true);

	// Applichiamo la spinta fisica (il lancio)
	if (AttachedChar)
	{
		// Prendiamo la direzione in avanti del character e aggiungiamo una componente Z (verso l'alto) per fare la parabola
		FVector ThrowDirection = AttachedChar->GetActorForwardVector() + FVector(0.f, 0.f, 0.4f);
		ThrowDirection.Normalize();

		// Modifica questo valore per regolare la potenza del lancio
		float ThrowStrength = 1200.f; 

		// AddImpulse spinge l'oggetto istantaneamente in base alla sua massa
		MeshComp->AddImpulse(ThrowDirection * ThrowStrength * MeshComp->GetMass());
	}
}