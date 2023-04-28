// Fill out your copyright notice in the Description page of Project Settings.
#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	CollisionChannel = ECC_WorldDynamic;
	
}

void USInteractionComponent::FindBestInteractable()
{
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* MyOwner = GetOwner();

	FVector2D ScreenSize;

	GetWorld()->GetGameViewport()->GetViewportSize(ScreenSize);

	FVector2D CrosshairLocation(ScreenSize.X / 2.f,ScreenSize.Y / 2.f);

	FVector CrosshairWorldLocation;
	FVector CrosshairWorldDirection;

	//2D vekt�r bilgisini 3D vekt�re �evirir b�ylelikle y�n bilgisi i�in bir de�i�kene daha ihtiyac�m�z var.
	UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
	CrosshairLocation, CrosshairWorldLocation, CrosshairWorldDirection);
	
	FVector CrosshairEnd = CrosshairWorldLocation + (CrosshairWorldDirection * 1000);
	

	FCollisionShape Shape;
	Shape.SetSphere(50.f);

	TArray<FHitResult> hits;

	GetWorld()->SweepMultiByObjectType(hits, CrosshairWorldLocation, CrosshairEnd,FQuat::Identity, QueryParams,Shape );
	//DrawDebugLine(GetWorld(), CrosshairWorldLocation, CrosshairEnd, FColor::Green, true, 1.0f);
	
	FocusedActor = nullptr;
	
	for (FHitResult bHit:hits)
	{
		AActor* HitActor = bHit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}

	if (FocusedActor)
	{
		if (DefaultWidgetInstance == nullptr && ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(),DefaultWidgetClass);
		}

		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachActor = FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}

}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
	
}

void USInteractionComponent::PrimaryInteract()
{
	ServerInteract(FocusedActor);
}
void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());
	ISGameplayInterface::Execute_Interact(InFocus, MyPawn);

}

