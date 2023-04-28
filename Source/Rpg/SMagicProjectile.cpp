// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SAttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SActionComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SActionEffect.h"
#include "USGameplayFunctionLibrary.h"


// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp->SetSphereRadius(20.0f);
	InitialLifeSpan = 10.0f;

	SphereComp->OnComponentHit.AddDynamic(this,&ASMagicProjectile::OnActorHit);
}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::PlaySoundAtLocation(this, Magic_Imapct, Hit.ImpactPoint);

	if (OtherActor && OtherActor != GetInstigator() && HasAuthority())
	{
		USActionComponent* ActionComp = Cast<USActionComponent>(Hit.GetActor()->GetComponentByClass(USActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MoveComp->Velocity = -MoveComp->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		
		if (UUSGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, 20.0f, Hit))
		{	
			Explode();
			
			if (ActionComp)
			{
				ActionComp->AddAction(BurningActionClass, Hit.GetActor());
			}
		}
	}
}



// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

