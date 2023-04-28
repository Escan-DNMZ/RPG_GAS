// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SSaveGame.generated.h"

USTRUCT()
struct FActorSaveData{

	GENERATED_BODY()

public:

	UPROPERTY()
	FString ActorName;

	//for movable actors keep Locatin,Rotation,Scale
	UPROPERTY()
	FTransform ActorTransform;

	UPROPERTY()
	TArray<uint8> ByteData;
	
};

USTRUCT()
struct FPlayerSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	int Points;

	UPROPERTY()
	FString PlayerID;

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;
	
};

UCLASS()
class RPG_API USSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
		int32 Point;

	UPROPERTY()
	TArray<FActorSaveData> SavedActor;

	UPROPERTY()
	TArray<FPlayerSaveData> SavedPlayer;
	
	FPlayerSaveData* GetPlayerData(APlayerState* PlayerState);
	
};
