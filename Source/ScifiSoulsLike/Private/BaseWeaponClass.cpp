// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeaponClass.h"

// Sets default values
ABaseWeaponClass::ABaseWeaponClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon Mesh");

	m_AttackSpeed = 1.0f;

	m_DamageAmount = 10.f;

}

// Called when the game starts or when spawned
void ABaseWeaponClass::BeginPlay()
{
	Super::BeginPlay();

	CheckLevel();
	
}

// Called every frame
void ABaseWeaponClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_StartSocket = m_WeaponMesh->GetSocketLocation(FName("WeaponStart"));
	m_EndSocket = m_WeaponMesh->GetSocketLocation(FName("WeaponEnd"));

}

void ABaseWeaponClass::CheckLevel()
{
	switch (m_CurrentWeaponLevel)
	{
	case EWeaponLevel::E_Level1:
		m_AttackSpeed = 1.0f;
		m_DamageAmount = 10.f;
		break;
	case EWeaponLevel::E_Level2:
		m_AttackSpeed = 1.5f;
		m_DamageAmount = 25.f;
		break;
	case EWeaponLevel::E_Level3:
		m_AttackSpeed = 2.0f;
		m_DamageAmount = 35.f;
		break;
	default:
		break;
	}
}

