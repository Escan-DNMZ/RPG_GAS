 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SActionComponent.generated.h"

class USAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Tags")
    FGameplayTagContainer ActiveGameplayTags;

	UFUNCTION(BlueprintCallable, Category = "Actions")
		void AddAction(TSubclassOf<USAction> ActionClass, AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Actions")
		 bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
		 bool StopActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BlueprintCallable, Category = "Actions")
		bool RemoveAction(USAction* Instigator);

	// Sets default values for this component's properties
	USActionComponent();


protected:

	UFUNCTION(Server,Reliable)
	void StartServerAction(AActor* Instigator, FName ActionName);

	
	UFUNCTION(Server,Reliable)
	void StopServerAction(AActor* Instigator, FName ActionName);
	
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Actions")
		TArray<TSubclassOf<USAction>> DefaultActions;

	UPROPERTY(EditAnywhere,Replicated, Category = "Actions")
	TArray<USAction*> Actions;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool ReplicateSubobjects(class UActorChannel *Channel, class FOutBunch *Bunch, FReplicationFlags *RepFlags) override;

};
