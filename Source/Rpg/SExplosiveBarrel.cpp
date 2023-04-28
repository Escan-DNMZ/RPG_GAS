// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	forceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	forceComp->SetupAttachment(MeshComp);

	forceComp->SetAutoActivate(false);
	
	forceComp->Radius = 750.0f;
	forceComp->ImpulseStrength = 2500.0f;
	forceComp->bImpulseVelChange = true;

	forceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);


}


// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &ASExplosiveBarrel::OnActorHit);
}

void ASExplosiveBarrel::OnActorHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	forceComp->FireImpulse();

	FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *OtherActor->GetActorLocation().ToString());
	

	GetWorldTimerManager().SetTimer(DestroyHandle, this, &ASExplosiveBarrel::Destroy, 1.0f);

	
}


void ASExplosiveBarrel::Destroy()
{
	GetWorld()->DestroyActor(this);
}

