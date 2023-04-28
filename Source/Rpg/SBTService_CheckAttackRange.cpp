// Fill out your copyright notice in the Description page of Project Settings.


#include "SBTService_CheckAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "SAttributeComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		if (BlackboardComp)
		{
		  AActor* TargetActor =	Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		  if (TargetActor)
		  {
			
			  AAIController* MyController = OwnerComp.GetAIOwner();
			  
			  if (ensure(MyController))
			  {
				  APawn* AIPawn = MyController->GetPawn();
				  if (AIPawn)
				  {
					  float DistanceTo = FVector::Distance(AIPawn->GetActorLocation(), TargetActor->GetActorLocation());
					  bool bWithinRange = DistanceTo < 2000.F;
					   
					 
					  bool bHasLos = false;
					  if (bWithinRange) {
						  bHasLos = MyController->LineOfSightTo(TargetActor);
					  }
						  

					  BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName,bWithinRange);


				
				  }
			  }
		  }
		}
}
