// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums.h"
#include "BaseWeaponClass.generated.h"

UCLASS()
class SCIFISOULSLIKE_API ABaseWeaponClass : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	ABaseWeaponClass();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,meta = (DisplayName = "WeaponMesh"), meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* m_WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "DamageVariable"))
	float m_DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "AttackSpeed"))
	float m_AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enum")
	EWeaponLevel m_CurrentWeaponLevel = EWeaponLevel::E_Level1;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector m_StartSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector m_EndSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float m_WeaponRadius;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void CheckLevel();

};
