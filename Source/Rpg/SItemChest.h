// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"


UCLASS()
class RPG_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
	float TargetPitch;

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	virtual void OnActorLoaded_Implementation() override;
	
	// Sets default values for this actor's properties
	ASItemChest();

	UPROPERTY(ReplicatedUsing="OnRep_LidOpened",BlueprintReadOnly,SaveGame)
	bool bLidOpened;

	UFUNCTION(BlueprintNativeEvent)
	void OnRep_LidOpened();
	 
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly) 
	UStaticMeshComponent* LidMesh;
	

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		class UParticleSystemComponent* UltiParticle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
