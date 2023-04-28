// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Tools/UEdMode.h"

void ASPlayerState::SavePlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		// Gather all relevant data for player
		FPlayerSaveData SaveData;
		SaveData.Points = Point;
		// Stored as FString for simplicity (original Steam ID is uint64)
		SaveData.PlayerID = GetUniqueId().ToString();

		// May not be alive while we save
		if (APawn* MyPawn = GetPawn())
		{
			SaveData.Location = MyPawn->GetActorLocation();
			SaveData.Rotation = MyPawn->GetActorRotation();
		}
		
		SaveObject->SavedPlayer.Add(SaveData);
	}
}

void ASPlayerState::LoadPlayerState_Implementation(USSaveGame* SaveObject)
{
	if (SaveObject)
	{
		AddCredits(SaveObject->Point);
	}
}

void ASPlayerState::OnRep_Point(int32 OldPoint)
{
	OnPointsChanged.Broadcast(this, Point, Point - OldPoint);
}

int32 ASPlayerState::GetPoints() const
{
	return Point;
}

void ASPlayerState::AddCredits(int32 Delta)
{
	if (!ensure(Delta >= 0.0f))
	{
		return;
	}
	
	Point += Delta;
	
	OnPointsChanged.Broadcast(this, Point, Delta);
}



bool ASPlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta >= 0.0f))
	{
		return false;
	}

	Point += Delta;
	
	OnPointsChanged.Broadcast(this, Point, Delta);
	return  true;
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Point);
}