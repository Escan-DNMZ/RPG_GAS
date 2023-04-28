// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"
#include "SProjectileBase.h"
#include "SCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();


}

void ASTeleportProjectile::Explode_Implementation()
{
	ActorToTeleport = GetInstigator();

	if (ActorToTeleport && HasAuthority())
	{
		
		ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
		//i did it with SetActorLocation(GetActorLocation()); but Tom Looman use TeleportTo

		UGameplayStatics::PlaySoundAtLocation(this,SoundImpact,GetActorLocation());
		Destroy();
	}
	
}

void ASTeleportProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASTeleportProjectile, ActorToTeleport);
}