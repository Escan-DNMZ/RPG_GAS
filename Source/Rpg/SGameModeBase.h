// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h" 
#include "Engine/DataTable.h"
#include "SGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class UDataTable;

USTRUCT(BlueprintType)
struct FHealthRow : public FTableRowBase {

	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	TSubclassOf<AActor> HealthClass;

};

UCLASS()
class RPG_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ASGameModeBase();

	virtual void StartPlay() override;

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	UDataTable* HealthTable;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		TSubclassOf<AActor> MinionClass;

	//UPROPERTY(EditDefaultsOnly, Category = "Health	")
	//	TSubclassOf<AActor> HealthClass;

	UPROPERTY(EditDefaultsOnly,Category = "AI")	
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly,Category = "AI")
	UCurveFloat* DifficultyCurve;

	FTimerHandle TimerHandle_SpawnBots;
	FTimerHandle TimerHandle;

	UPROPERTY(EditDefaultsOnly,Category = "AI")
	float SpawnTimerInterval;

	UFUNCTION()
	void SpawnBotTimerElapsed();
	
	UFUNCTION()
	void SpawnHealth();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,EEnvQueryStatus::Type QueryStatus);

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	FString SlotName;
	
	UPROPERTY()
	class USSaveGame* CurrentSaveGame;

	UFUNCTION(BlueprintCallable,Category="SaveGame")
	void WriteSaveGame();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	void LoadSaveGame();

	
	
};
