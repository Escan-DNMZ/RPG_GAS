// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "SCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(GetRootComponent());

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	UltiParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	UltiParticle->SetupAttachment(BaseMesh);
	

	bReplicates = true;
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	if (HasAuthority())
	{
		bLidOpened = !bLidOpened;
		OnRep_LidOpened();
	}

	
}

void ASItemChest::OnActorLoaded_Implementation()
{
	ISGameplayInterface::OnActorLoaded_Implementation();

	OnRep_LidOpened();
}


void ASItemChest::OnRep_LidOpened_Implementation()
{
}

// Called when the game starts or when spawned
void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASItemChest, bLidOpened);

}