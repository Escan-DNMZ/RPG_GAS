// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SUltiProjectile.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ASUltiProjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
	
	virtual void Explode_Implementation() override;
public:
	ASUltiProjectile();

};
