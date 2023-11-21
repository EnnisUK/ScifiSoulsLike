// Copyright Epic Games, Inc. All Rights Reserved.

#include "ScifiSoulsLikeGameMode.h"
#include "UObject/ConstructorHelpers.h"

AScifiSoulsLikeGameMode::AScifiSoulsLikeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/bp_MainCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
