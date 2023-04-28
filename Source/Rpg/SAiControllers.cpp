// Fill out your copyright notice in the Description page of Project Settings.


#include "SAiControllers.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"


void ASAiControllers::BeginPlay()
{
	Super::BeginPlay();
	
	

	RunBehaviorTree(UBehaviorTree);

	//APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this,0);

	//if (MyPawn)
	//{
	//	GetBlackboardComponent()->SetValueAsVector("MoveToLocation",MyPawn->GetActorLocation());

	//	GetBlackboardComponent()->SetValueAsObject("TargetActor",MyPawn);
	//}
}
