// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPlayerState.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

UCLASS()
class RPG_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool UltiExist;

	FVector Origin;

	FVector BoxExtent;

	FVector BoxOrigin;

	void Ulti();

	FTimerHandle HealTimer;

	void MoveForward(float Axis);
	void MoveRight(float Axis);

	void PrimaryAttack();

	void PrimaryInteract();

	void Dash();

	void SprintStart();
	
	void SprintStop();

	void Heal();

	void Heal_Ending();

	bool IsDead;

	UFUNCTION()
		void OnHealtChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comp")
	class USActionComponent* ActionComp;
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere)
	class USInteractionComponent* InteractComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Comp")
	TObjectPtr<USAttributeComponent> AttributeComp; 

	ASPlayerState* PlayerState;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
