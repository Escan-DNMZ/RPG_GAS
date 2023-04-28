// Fill out your copyright notice in the Description page of Project Settings.


#include "SAiCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SAttributeComponent.h"
#include "BrainComponent.h"
#include "SPlayerState.h"
#include "SWorldUserWidget.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ASAiCharacter::ASAiCharacter()
{
 	

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("Sensing Componenet");
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAiCharacter::OnPawnSeen);

	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>("Attribute Component");
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASAiCharacter::OnHealthChanged);
	

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
} 

void ASAiCharacter::PostInitializeComponenets()
{
	Super::PostInitializeComponents();

	
}

void ASAiCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		if (NewHealth <= 0.0f) {

			AAIController* AIC = Cast<AAIController>(GetController());
			if (AIC)
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}

			if (ActiveHealtBar == nullptr)
			{
				ActiveHealtBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
				if (ActiveHealtBar)
				{
					ActiveHealtBar->AddToViewport();
				}
			
			}
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");

			
			SetLifeSpan(10.0f);
		}
	}
}

void ASAiCharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
	  UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();
		
	  BBComp->SetValueAsObject("TargetActor",Pawn);
	}
}



