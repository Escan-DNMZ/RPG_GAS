// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "SInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "SAttributeComponent.h"
#include "HealthPotion.h"
#include "Kismet/KismetMathLibrary.h"
#include "SActionComponent.h"
#include "SGameModeBase.h"
#include "SPlayerState.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm); 

	InteractComponent = CreateDefaultSubobject<USInteractionComponent>("InteractComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealtChanged);
	
	
	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	SetPlayerState(PlayerState);

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	
}

void ASCharacter::Ulti()
{
	if(UltiExist){
		ActionComp->StartActionByName(this, "Ulti");
	}

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	GetActorBounds(false, Origin, BoxExtent);
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	BoxOrigin = UKismetMathLibrary::RandomPointInBoundingBox(Origin, BoxExtent);
}

void ASCharacter::PrimaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}

void ASCharacter::Dash()
{
	ActionComp->StartActionByName(this, "Teleport");
}

void ASCharacter::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void ASCharacter::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void ASCharacter::Heal()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	DisableInput(PC);
	

	GetWorldTimerManager().SetTimer(HealTimer, this, &ASCharacter::Heal_Ending, 1.5f);
	
	
	
}

void ASCharacter::Heal_Ending() {

	APlayerController* PC = Cast<APlayerController>(GetController());
	EnableInput(PC);

	if (InteractComponent) {
		
		InteractComponent->PrimaryInteract();
	}
}


void ASCharacter::PrimaryInteract()
{

	if (InteractComponent)
	{
	
		InteractComponent->PrimaryInteract();

	}

}


void ASCharacter::OnHealtChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta) {

	
	 
	// Died
	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
		
		IsDead = true;
		

		SetLifeSpan(5.0f);
	}

}


// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",this,&ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Primary", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ASCharacter::Dash);

	PlayerInputComponent->BindAction("Ulti", IE_Pressed, this, &ASCharacter::Ulti);

	PlayerInputComponent->BindAction("Heal", IE_Pressed, this, &ASCharacter::Heal);


	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintStop);
}

void ASCharacter::MoveForward(float Axis)
{
	FRotator Rotator = GetControlRotation();
	Rotator.Pitch = 0.0f;
	Rotator.Roll = 0.0f;

	AddMovementInput(Rotator.Vector(),Axis);
}

void ASCharacter::MoveRight(float Axis)
{
	FRotator Rotator = GetControlRotation();
	Rotator.Pitch = 0.0f;
	Rotator.Roll = 0.0f;

	FVector GetRight = FRotationMatrix(Rotator).GetScaledAxis(EAxis::Y);

	AddMovementInput(GetRight, Axis);
}

