// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckHealth.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "SAttributeComponent.h"





void UBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BC = OwnerComp.GetBlackboardComponent();
	if (BC)
	{
		APawn* TargetActor = Cast<APawn>(BC->GetValueAsObject("TargetActor"));

		if (TargetActor)
		{
		
			bool LowHealth = false;
			USAttributeComponent* Attribute = Cast<USAttributeComponent>(TargetActor->GetComponentByClass(USAttributeComponent::StaticClass()));
			if (Attribute)
			{
				if (Attribute->Health < 50)
				{
					LowHealth = true;
					BC->SetValueAsBool(LowHp.SelectedKeyName, LowHealth);
				}

			}
		}
	}

	
}
