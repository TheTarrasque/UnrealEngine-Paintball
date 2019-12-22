// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Fog.h"
#include "NXL_Player.generated.h"

/**
 * 
 */
UCLASS()
class NXL2019_LUCIANO_API ANXL_Player : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		AFog* FoW;
protected:
	virtual void BeginPlay();
	
	
};
