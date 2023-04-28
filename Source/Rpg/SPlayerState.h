// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSaveGame.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPointChanged, ASPlayerState*, PlayerState, int32, NewCredits, int32, Delta);
UCLASS()
class RPG_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(USSaveGame* SaveObject);

	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(USSaveGame* SaveObject);

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing="OnRep_Point",SaveGame)
	int32 Point;

	UFUNCTION()
	void OnRep_Point(int32 OldPoint);

	UPROPERTY(BlueprintAssignable)
	FOnPointChanged OnPointsChanged;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetPoints() const;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Delta);
};
