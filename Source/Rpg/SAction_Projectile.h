// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_Projectile.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API USAction_Projectile : public USAction
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
		TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
		FName HandSocketName;

	UPROPERTY(EditAnywhere, Category = "Attack")
		float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Attack")
		class UAnimMontage* AttackAnim;

	UPROPERTY(EditAnywhere, Category = "Attack")
		class UParticleSystem* MuzzleFlash;

	UFUNCTION()
		void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

	ACharacter* Character;
public:

	virtual void StartAction_Implementation(AActor* Instigator) override;

	USAction_Projectile();
	
};
