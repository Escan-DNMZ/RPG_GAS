// Fill out your copyright notice in the Description page of Project Settings.


#include "USGameplayFunctionLibrary.h"

#include "SAttributeComponent.h"

bool UUSGameplayFunctionLibrary::ApplyDamage(AActor* DamageCause, AActor* TargetActor, float DamageAmount)
{
	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		return  AttributeComp->ApplyHealthChange(DamageCause, -DamageAmount);
	}
	return false;
}

bool UUSGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser,TargetActor,DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();

			HitComp->AddImpulseAtLocation(Direction*300000.f,HitResult.ImpactPoint,HitResult.BoneName);
		}
		return  true;
	}
	return false;
}
