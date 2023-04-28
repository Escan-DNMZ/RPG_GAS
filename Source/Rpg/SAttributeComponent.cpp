// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SGameModeBase.h"
#include "Math/UnrealMathUtility.h"
#include "Net/UnrealNetwork.h"

class ASGameModeBase;
// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	MaxHealth = 100;
	Health = MaxHealth;

	SetIsReplicatedByDefault(true);
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return FromActor->FindComponentByClass<USAttributeComponent>();
	}
	return nullptr;
}


// Called when the game starts
void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor,this,NewHealth,Delta);
}

bool USAttributeComponent::IsAlive() const {

	return Health > 0.0f;
}


bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor,float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}
	
	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta,0.0f,MaxHealth);
	float ActualDelta = NewHealth - OldHealth;
	
	//Is Server ? 
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;
		
		if (ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}

		// Died
		if (ActualDelta < 0.0f && Health == 0.0f)
		{
			ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
			if (GM){
			
				UE_LOG(LogTemp, Warning, TEXT("OnActorKilled."));
				GM->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}
	return ActualDelta != 0;
}
void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent,Health);
	DOREPLIFETIME(USAttributeComponent,MaxHealth);
}

