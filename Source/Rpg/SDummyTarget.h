// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SDummyTarget.generated.h"

UCLASS()
class RPG_API ASDummyTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDummyTarget();

protected:

	UPROPERTY(EditAnywhere)
	 class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	class USAttributeComponent* AttributeComp;

	UFUNCTION()
	void OnHealtChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	
public:	
	


};
