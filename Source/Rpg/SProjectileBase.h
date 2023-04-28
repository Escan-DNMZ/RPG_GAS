// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

UCLASS()
class RPG_API ASProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASProjectileBase();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Components")
		class UParticleSystem* ImpactVfx;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		class USphereComponent* SphereComp;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UProjectileMovementComponent* MoveComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	class UParticleSystemComponent* EffectComp;

	

	UFUNCTION()
		virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Explode();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* Magic_Imapct;

	virtual void PostInitializeComponents() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
