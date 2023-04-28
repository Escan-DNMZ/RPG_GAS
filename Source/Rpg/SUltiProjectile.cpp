// Fill out your copyright notice in the Description page of Project Settings.


#include "SUltiProjectile.h"
#include "SProjectileBase.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "SAttributeComponent.h"
#include "USGameplayFunctionLibrary.h"
#include "Components/CapsuleComponent.h"


ASUltiProjectile::ASUltiProjectile()
{
	
}

void ASUltiProjectile::Explode_Implementation()
{

	FVector Radial = SphereComp->GetComponentLocation();
	FVector End = Radial;
	TArray<FHitResult> Hits;
	float Radius = 750.f;
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(Radius);

	bool SweepHit = GetWorld()->SweepMultiByChannel(Hits, Radial,End,FQuat::Identity,ECC_WorldDynamic, SphereShape);

	if (SweepHit)
	{
		for (FHitResult hit:Hits) {

			UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(hit.GetActor()->GetRootComponent());
			
			if (Mesh)
			{
				Mesh->AddRadialImpulse(Radial, Radius, 2000.f, ERadialImpulseFalloff::RIF_Constant, true);
				
			}
			for (FHitResult Hit: Hits){
				if (UUSGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), Hit.GetActor(), 20.0f, hit))
				{
					Explode();
				}
			}
		}
	}
}


