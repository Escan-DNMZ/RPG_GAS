// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckHealth.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBTService_CheckHealth : public UBTService
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector LowHp;

	

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
