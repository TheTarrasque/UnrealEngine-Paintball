// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "NXL2019_lucianoHUD.generated.h"

UCLASS()
class ANXL2019_lucianoHUD : public AHUD
{
	GENERATED_BODY()

public:
	ANXL2019_lucianoHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

