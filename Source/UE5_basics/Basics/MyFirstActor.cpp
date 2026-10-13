// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFirstActor.h"

// Sets default values
AMyFirstActor::AMyFirstActor()
{
 	PrimaryActorTick.bCanEverTick = true; // can tick?
	PrimaryActorTick.TickInterval = 0.2f; // what is the interval tick?

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	/*	
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetBoxExtent(FVector(200.f, 200.f, 100.f));
	BoxComponent->SetCanEverAffectNavigation(false);
	*/
	BoxComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMyFirstActor::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &AMyFirstActor::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AMyFirstActor::OnEndOverlap);

	// Bind example
	/*
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AFP_FirstPersonCharacter * FPCharacter = dynamic_cast<AFP_FirstPersonCharacter*>(PlayerCharacter);
    if(IsValid(FPCharacter))
    {
    	FPCharacter->OnLineTraceCast.AddDynamic(this, &AMyFirstActor::BindTest);
    }
    */
}

/*void AMyFirstActor::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if(!IsValid(OtherActor))
	{
		return;
	}

	UClass* OtherActorClass = OtherActor->GetClass();
	for(auto& SensedClass : SensedCharacterClasses)
	{
		if(SensedClass == OtherActorClass || OtherActorClass->IsChildOf(SensedClass))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "IN: "+OtherActor->GetName());
			//OnSomethingEnter();
			break;
		}
	}
	
	/*
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}
	
	if(OtherActor == UGameplayStatics::GetPlayerPawn(World, 0))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "IN: "+OtherActor->GetName());
	}
	*/
//}

void AMyFirstActor::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if(!IsValid(OtherActor))
	{
		return;
	}

	UClass* OtherActorClass = OtherActor->GetClass();
	for(auto& SensedClass : SensedCharacterClasses)
	{
		if(SensedClass == OtherActorClass || OtherActorClass->IsChildOf(SensedClass))
		{
			// Se non è già presente nell'array, lo aggiungiamo
			if(!TrackedActors.Contains(OtherActor))
			{
				TrackedActors.Add(OtherActor);
                
				// Colloquio di lavoro: ci leghiamo all'evento OnDestroyed per evitare puntatori morti!
				OtherActor->OnDestroyed.AddDynamic(this, &AMyFirstActor::OnTargetDestroyed);

				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "SPAWN/IN: " + OtherActor->GetName());
			}
			break;
		}
	}
}

/*void AMyFirstActor::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if(!IsValid(OtherActor))
	{
		return;
	}

	UClass* OtherActorClass = OtherActor->GetClass();
	for(auto& SensedClass : SensedCharacterClasses)
	{
		if(SensedClass == OtherActorClass || OtherActorClass->IsChildOf(SensedClass))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "OUT: "+OtherActor->GetName());
		}
	}

	/*
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		return;
	}
	
	if(OtherActor == UGameplayStatics::GetPlayerPawn(World, 0))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "OUT: "+OtherActor->GetName());
	}
	*/
//}

void AMyFirstActor::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if(!IsValid(OtherActor))
	{
		return;
	}

	if(TrackedActors.Contains(OtherActor))
	{
		TrackedActors.Remove(OtherActor);
		OtherActor->OnDestroyed.RemoveDynamic(this, &AMyFirstActor::OnTargetDestroyed);

		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "OUT: " + OtherActor->GetName());
	}
}

void AMyFirstActor::OnTargetDestroyed(AActor* DestroyedActor)
{
	if(IsValid(DestroyedActor))
	{
		TrackedActors.Remove(DestroyedActor);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, "DESTROY: " + DestroyedActor->GetName());
	}
}

void AMyFirstActor::BindTest()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Purple, "Player has shoot!");
}

// Called every frame
/*void AMyFirstActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//GEngine->AddOnScreenDebugMessage(896745, 2.f, FColor::Green, FString::Printf(TEXT("world time: %f"), GetWorld()->TimeSeconds));
	//GEngine->AddOnScreenDebugMessage(8967444, 2.f, FColor::Blue, "world time: "+FString::SanitizeFloat(GetWorld()->TimeSeconds));
}*/

void AMyFirstActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Tasto di debug: premi F1 per stampare la lista degli attori tracciati
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if(PC && PC->WasInputKeyJustPressed(EKeys::P))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "=== LISTA ATTORI DENTRO MYFIRSTACTOR ===");
        
		// Pulizia di sicurezza e stampa
		for(int32 i = TrackedActors.Num() - 1; i >= 0; --i)
		{
			if(IsValid(TrackedActors[i]))
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TrackedActors[i]->GetName());
			}
			else
			{
				// Se per qualche motivo c'è un elemento non valido, lo rimuoviamo al volo
				TrackedActors.RemoveAt(i);
			}
		}
	}
}
