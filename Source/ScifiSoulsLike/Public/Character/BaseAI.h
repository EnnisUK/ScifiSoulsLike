// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DecalActor.h"
#include "AIInterface.h"
#include "BaseAI.generated.h"


UCLASS()
class SCIFISOULSLIKE_API ABaseAI : public ACharacter, public IAIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseAI();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UDecalComponent* m_LockedOnDecal;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "IsTakingDamage"))
	bool m_IsTakingDamage;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "CurrentHealth"))
	float m_CurrentHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "MaxHealth"))
	float m_MaxHealth = 100;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void Show_HideDecal(UDecalComponent* LockedOnDecal, bool ShouldHide);

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float Damage);


	virtual void Lockon_Implementation() override;

	virtual void EndLockon_Implementation() override;

	virtual void Damage_Implementation(float Damage) override;

	virtual void Death_Implementation() override;

};
