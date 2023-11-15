// Copyright Epic Games, Inc. All Rights Reserved.

#include "ScifiSoulsLikeGameMode.h"
#include "ScifiSoulsLikeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AScifiSoulsLikeGameMode::AScifiSoulsLikeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
