// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "NXL2019_lucianoGameMode.h"
#include "NXL2019_lucianoHUD.h"
#include "NXL2019_lucianoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANXL2019_lucianoGameMode::ANXL2019_lucianoGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ANXL2019_lucianoHUD::StaticClass();
}
