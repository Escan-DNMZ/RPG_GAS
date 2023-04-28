// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPotion.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

#define LOCTEXT_NAMESPACE ""

// Sets default values
AHealthPotion::AHealthPotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(GetRootComponent());
	BaseMesh->SetSimulatePhysics(true);

	PointCosts = 50;

	bReplicates = true;
}

void AHealthPotion::Interact_Implementation(APawn* InstigatorPawn) 
{
	Destroy();
	
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (AttributeComp)
	{
		if (ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>())
		{
			PS->RemoveCredits(PointCosts);
			AttributeComp->ApplyHealthChange(this, AttributeComp->MaxHealth - AttributeComp->Health);
		}
		
	}
	
}

FText AHealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{ 
	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (AttributeComp && AttributeComp->Health >= 100)
	{
		return LOCTEXT("HealthPotion_FullHealthWarning", "Already at full Health");
	}
	return FText::Format(LOCTEXT("HealthPotion_InteractMessage", "Point {0} Points, Restores health to maximum"), PointCosts);
}

// Called when the game starts or when spawned
void AHealthPotion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealthPotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


#undef LOCTEXT_NAMESPACE