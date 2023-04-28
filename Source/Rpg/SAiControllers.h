// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SAiControllers.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ASAiControllers : public AAIController
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* UBehaviorTree;

	virtual void BeginPlay() override;
};
