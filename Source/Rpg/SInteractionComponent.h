// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWorldUserWidget.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"

class USWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()
public:

	void PrimaryInteract();

public:	
	// Sets default values for this component's properties
	USInteractionComponent();

protected:

	void FindBestInteractable();

	//Reliable - TCP/IP Critical 
	//UnReliable - UDP Not important like Camera Shake
	UFUNCTION(Server,Reliable)
	void ServerInteract(AActor* InFocus);
	
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<AActor> FocusedActor;

	UPROPERTY(EditDefaultsOnly,Category="UI")
	TSubclassOf<USWorldUserWidget> DefaultWidgetClass;

	UPROPERTY()
	TObjectPtr<USWorldUserWidget> DefaultWidgetInstance;

	UPROPERTY(EditDefaultsOnly,Category="Trace")
	TEnumAsByte<ECollisionChannel> CollisionChannel;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
 
		
};
