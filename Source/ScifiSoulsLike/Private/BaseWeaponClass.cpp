// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeaponClass.h"

// Sets default values
ABaseWeaponClass::ABaseWeaponClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon Mesh");

	m_AttackSpeed = 1.0f;

	

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

}

