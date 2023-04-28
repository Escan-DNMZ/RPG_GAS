// Fill out your copyright notice in the Description page of Project Settings.

#include "SActionComponent.h"
#include "SAction.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"


USActionComponent::USActionComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}




// Called when the game starts
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->HasAuthority())
	{
		for (TSubclassOf<USAction> ActionClass:DefaultActions)
		{
			AddAction(ActionClass,GetOwner());
		}
	}
	

	
}


// Called every frame
void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool USActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for (USAction* Action : Actions)
	{
		if (Action)
		{
			WroteSomething |= Channel->ReplicateSubobject(Action,*Bunch,*RepFlags);
		}
	}
	return WroteSomething;
}

void USActionComponent::AddAction(TSubclassOf<USAction> ActionClass, AActor* Instigator)
{
	if (!ensure(ActionClass))
	{
		return;
	}

	USAction* NewAction = NewObject<USAction>(GetOwner(),ActionClass);
	if (NewAction)
	{
		NewAction->Initialize(this);
		Actions.Add(NewAction);

		if (NewAction->bAutoStart && ensure(NewAction->CanAction(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
}


bool USActionComponent::RemoveAction(USAction* Instigator)
{
	
	Actions.Remove(Instigator);
	
	return false;
}


bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!Action->CanAction(Instigator))
			{
				continue;
			}

			if (!GetOwner()->HasAuthority())
			{
				StartServerAction(Instigator,ActionName);
				
			}
			
			
			Action->StartAction(Instigator);
			return true;
		}
	}
	return false;
}
void USActionComponent::StopServerAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator,ActionName);
}
void USActionComponent::StartServerAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator,ActionName);
}


bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for (USAction* Action : Actions)
	{
		if (Action && Action->ActionName == ActionName)
		{
			if (!GetOwner()->HasAuthority())
			{
				StopServerAction(Instigator, ActionName);
			}
			
			if (Action->IsTeleporting()){
				Action->StopAction(Instigator);
				return true;
			}
			
		}
	}
	return false;
}

void USActionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USActionComponent,Actions);


}
