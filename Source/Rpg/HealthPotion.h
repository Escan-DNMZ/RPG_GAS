// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "HealthPotion.generated.h"

UCLASS()
class RPG_API AHealthPotion : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

		
	
public:	
	// Sets default values for this actor's properties
	AHealthPotion();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	FText GetInteractText_Implementation(APawn* InstigatorPawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	int PointCosts;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* BaseMesh;
	
	UPROPERTY(EditAnywhere)
	class TSubclassOf<UActorComponent> Attribute;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
