// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAiCharacter.generated.h"

class UPawnSensingComponent;
class USerWidget;
class USWorldUserWidget;

UCLASS()
class RPG_API ASAiCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAiCharacter();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> HealthBarWidgetClass;

	USWorldUserWidget* ActiveHealtBar;

	virtual void PostInitializeComponenets();
	 
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditAnywhere)
	class USAttributeComponent* AttributeComponent;
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
};
