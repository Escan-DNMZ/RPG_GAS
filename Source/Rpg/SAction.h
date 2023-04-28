// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "SActionComponent.h"
#include "SAction.generated.h"


/**
 * 
 */

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsTeleporting;

	UPROPERTY(NotReplicated)
	AActor* Instigator;
};


UCLASS(Blueprintable)
class RPG_API USAction : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated)
	USActionComponent* ActionComp;
	
	UFUNCTION(BlueprintCallable, Category = "Action")
		USActionComponent* GetOwningComponent() const;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
		FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
		FGameplayTagContainer Blocked;


	

public:

	void Initialize(USActionComponent* NewActionComp);

	UPROPERTY(EditDefaultsOnly,Category = "Action")
	bool bAutoStart;

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanAction(AActor* Intigator);

	UFUNCTION(BlueprintNativeEvent,Category = "Action")
	 void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "Action")
	 void StopAction(AActor* Instigator);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
		FName ActionName;

	class UWorld* GetWorld() const override;

	UFUNCTION(BlueprintCallable, Category = "Action")
		bool IsTeleporting();

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;

	UFUNCTION()
	void OnRep_RepData();

	virtual bool IsSupportedForNetworking() const override;

	
};
