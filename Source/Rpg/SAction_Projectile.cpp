// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_Projectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "SCharacter.h"

USAction_Projectile::USAction_Projectile()
{
	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;
}

void USAction_Projectile::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	Character = Cast<ACharacter>(Instigator);

	if (Character)
	{
		
		Character->PlayAnimMontage(AttackAnim);

		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		if (Character->HasAuthority())
		{
			FTimerHandle TimerHand;
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);
			GetWorld()->GetTimerManager().SetTimer(TimerHand, Delegate, AttackAnimDelay, false);
		}

	}
}


void USAction_Projectile::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if (ensureAlways(ProjectileClass))
	{
		
	
	Character = Cast<ACharacter>(InstigatorCharacter);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//Se�ili pawn'�n hasar g�rmesini engelliyor
	SpawnParams.Instigator = InstigatorCharacter;

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(InstigatorCharacter);

	FCollisionShape Shape;
	Shape.SetSphere(20.f);

	FVector Start = Character->GetPawnViewLocation();
	FVector End = Start + Character->GetControlRotation().Vector() * 50000.f;

	FHitResult Hit;
	if (GetWorld()->SweepSingleByObjectType(Hit, Start, End, FQuat::Identity, ObjParams, Shape, Params)) {
		End = Hit.ImpactPoint;
	}

	FVector HandLocation = Character->GetMesh()->GetSocketLocation(HandSocketName);

	FRotator ProjRotation = FRotationMatrix::MakeFromX(End - HandLocation).Rotator();
	FTransform SpawnIM = FTransform(ProjRotation, HandLocation);
	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnIM, SpawnParams);
	}

	StopAction(InstigatorCharacter);

}
