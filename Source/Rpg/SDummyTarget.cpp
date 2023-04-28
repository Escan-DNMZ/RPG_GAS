// Fill out your copyright notice in the Description page of Project Settings.


#include "SDummyTarget.h"
#include "SAttributeComponent.h"
// Sets default values
ASDummyTarget::ASDummyTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("Attribute");
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASDummyTarget::OnHealtChanged);

}



void ASDummyTarget::OnHealtChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		Mesh->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	}
	
}

