// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "SAiCharacter.h"
#include "SAttributeComponent.h"
#include "EngineUtils.h"
#include "HealthPotion.h"

#include "GameFramework/Actor.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "SSaveGame.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

ASGameModeBase::ASGameModeBase()
{

	SpawnTimerInterval = 2.0f;

	SlotName = "SaveGame01";

	PlayerStateClass = ASPlayerState::StaticClass();
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	

	
	
	//GetWorldTimerManager().SetTimer(TimerHandle, this, &ASGameModeBase::SpawnHealth, 10.0f, true);

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots,this,&ASGameModeBase::SpawnBotTimerElapsed,SpawnTimerInterval,true);
}

void ASGameModeBase::SpawnHealth() {
	ASCharacter* Character = Cast<ASCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	int32 NrOfPotion = 0;

	if (HealthTable)
	{
		TArray<FHealthRow*> HealthRows;
		HealthTable->GetAllRows("",HealthRows);


		int32 RandomIndex = FMath::RandRange(0, HealthRows.Num() - 1);
		FHealthRow* SelectedRow = HealthRows[RandomIndex];

		GetWorld()->SpawnActor<AActor>(SelectedRow->HealthClass, Character->BoxOrigin, FRotator::ZeroRotator);
	}
	

	//Oyun i�erisindeki t�m AHealthPotionlar� al�yor ayn� GetAllActorOfClass gibi
	for (TActorIterator<AHealthPotion> It(GetWorld()); It; ++It)
	{
		AHealthPotion* health = *It;

		NrOfPotion++;
	}
	int32 MaxPotion = 10;


	if (NrOfPotion >= MaxPotion )
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		return;
	}

}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	

	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAiCharacter> It(GetWorld()); It; ++It)
	{
		ASAiCharacter* Bot = *It;

		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Bot->GetComponentByClass(USAttributeComponent::StaticClass()));

		if (AttributeComp && AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}

	}



	float MaxBotCount = 10.0f;

	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	if (NrOfAliveBots >= MaxBotCount)
	{
		return;
	}


	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	// QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&ASGameModeBase::OnQueryCompleted);
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{

	
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	
	ASPlayerState* PS = NewPlayer->GetPlayerState<ASPlayerState>();

	if (ensure(PS))
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}

	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void ASGameModeBase::WriteSaveGame()
{
	for (int i = 0; i < GameState->PlayerArray.Num();i++)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break;
		}
	}

	CurrentSaveGame->SavedActor.Empty();
	
	//FActorIterator When game started it's give us in game actors
	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;

		if (!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}
		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetName();
		ActorData.ActorTransform = Actor->GetActorTransform();

		FMemoryWriter NewWriter(ActorData.ByteData);
		FObjectAndNameAsStringProxyArchive Ar(NewWriter,true);
		//Find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;
		//Converts Actor's SaveGame UPROPERTIES into binary system
		Actor->Serialize(Ar);
		
		CurrentSaveGame->SavedActor.Add(ActorData);
	}
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame,SlotName,0);
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}

void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName,0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName,0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp,Warning,TEXT("Failed to load SaveGame"));
			return;
		}
		UE_LOG(LogTemp,Warning,TEXT("LoadedSaveGame"));


		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;

			if (!Actor->Implements<USGameplayInterface>())
			{
				continue;
			}

			for (FActorSaveData ActorData:CurrentSaveGame->SavedActor)
			{
				if (ActorData.ActorName == Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.ActorTransform);

					FMemoryReader NewReader(ActorData.ByteData);
					FObjectAndNameAsStringProxyArchive Ar(NewReader,true);
					Ar.ArIsSaveGame = true;
					//Convert Binary array to actor's variable
					Actor->Serialize(Ar);

					ISGameplayInterface::Execute_OnActorLoaded(Actor);
					
					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
		UE_LOG(LogTemp,Warning,TEXT("Created new SaveGame"));
	}

	
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
	

	
	
	

	APawn* KillerPawn = Cast<APawn>(Killer);
	
	if (KillerPawn && KillerPawn != VictimActor)
	{
		
		ASPlayerState* PS = KillerPawn->GetPlayerState<ASPlayerState>();
		if (PS) 
		{
			UE_LOG(LogTemp, Warning, TEXT("Credit Added."));
			PS->AddCredits(20);
		}
	}
}
