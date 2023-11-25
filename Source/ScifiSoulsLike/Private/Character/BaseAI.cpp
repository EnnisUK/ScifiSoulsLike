// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseAI.h"
#include "Components/DecalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ABaseAI::ABaseAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_CurrentHealth = m_MaxHealth;

	m_WeaponChildClass = CreateDefaultSubobject<UChildActorComponent>("WeaponChildClass");

}

// Called when the game starts or when spawned
void ABaseAI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

}

// Called to bind functionality to input
void ABaseAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseAI::Show_HideDecal(UDecalComponent* LockedOnDecal, bool ShouldShow)
{
	if (ShouldShow)
	{
		LockedOnDecal->SetHiddenInGame(false);
	}
	else
	{
		LockedOnDecal->SetHiddenInGame(true);
	}
}

void ABaseAI::TakeDamage(float Damage)
{
	m_CurrentHealth = FMath::Clamp(m_CurrentHealth -= Damage, 0.0, m_MaxHealth);

	if (m_CurrentHealth == 0)
	{
		IAIInterface::Execute_Death(this);
	}
	

		
	
	

}

void ABaseAI::DeathFunction()
{
	GetCharacterMovement()->DisableMovement();

	if (m_DeathMontage)
	{
		PlayAnimMontage(m_DeathMontage);
	}
}

void ABaseAI::Lockon_Implementation()
{
}

void ABaseAI::EndLockon_Implementation()
{
}

void ABaseAI::Damage_Implementation(float Damage)
{
}

void ABaseAI::Death_Implementation()
{
}






