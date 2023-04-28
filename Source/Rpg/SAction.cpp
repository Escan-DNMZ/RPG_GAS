// Fill out your copyright notice in the Description page of Project Settings.
#include "SAction.h"

#include "Rpg.h"
#include "Engine/Engine.h"
#include "SActionComponent.h"
#include "Net/UnrealNetwork.h"


void USAction::StartAction_Implementation(AActor* Instigator)
{
	LogOnScreen(this,FString::Printf(TEXT("Started: %s"),*ActionName.ToString()),FColor::Green);

	USActionComponent* Comp = GetOwningComponent();

	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	RepData.bIsTeleporting = true;
	RepData.Instigator = Instigator;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	LogOnScreen(this,FString::Printf(TEXT("Stopped: %s"),*ActionName.ToString()),FColor::Green);

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsTeleporting = false;
	RepData.Instigator = Instigator;
}

bool USAction::CanAction_Implementation(AActor* Intigator)
{
	USActionComponent* Comp = GetOwningComponent();

	if (IsTeleporting()){
		return false;
	}

	if (Comp->ActiveGameplayTags.HasAny(Blocked))
	{
		return false;
	}
	return true;
}


USActionComponent* USAction::GetOwningComponent() const
{
	return ActionComp;
}

void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}


bool USAction::IsTeleporting()
{
	return RepData.bIsTeleporting;
}

UWorld* USAction::GetWorld() const
{
	AActor* Comp = Cast<AActor>(GetOuter());
	if (Comp)
	{
		return Comp->GetWorld();
	}
	return nullptr;
}

void USAction::OnRep_RepData()
{
	if (RepData.bIsTeleporting)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

bool USAction::IsSupportedForNetworking() const
{
	return true;
}


void USAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, ActionComp);
}

