// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "STeleportProjectile.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ASTeleportProjectile : public ASProjectileBase
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	
	virtual void Explode_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* SoundImpact;

	UPROPERTY(EditAnywhere,Replicated, BlueprintReadWrite)
	AActor* ActorToTeleport;

	
};
