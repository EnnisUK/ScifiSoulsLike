// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ScifiSoulsLikeCharacter.generated.h"


UCLASS(config=Game)
class AScifiSoulsLikeCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	//Sprinting Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;

	// Teleport Ability
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* TeleportAction;

public:
	AScifiSoulsLikeCharacter();

	// Stamina Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Stamina"))
	float m_Stamina;

	float m_MaxStamina = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Stamina Increase Amount"))
	float m_StaminaIncrease;

	bool m_IsSprinting;

	// Health Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Health"))
	float m_Health;

	float m_MaxHealth = 100.f;

	// Energy Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Energy"))
	float m_Energy;

	float m_MaxEnergy = 100.f;

	//Teleport Ability Variables
	UPROPERTY(EditAnywhere)
	float m_TeleportDistance = 500.f;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool m_IFrames;

	

	// Timer Handles For Character

	FTimerHandle m_DrainStaminaHandle;
	
	FTimerHandle m_DrainEnergyHandle;

	FTimerHandle m_RegenStaminaTimer;
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// Called For Starting Sprint
	void StartSprint();

	// Called For Ending Sprint
	void EndSprint();


	// Functions For Stamina

	void DrainStamina();

	void RegenStamina();

	// Functions for Energy;

	void DrainEnergy();

	void RegenEnergy();
	
	void CheckVelocity();

	void TeleportInput();

	void TeleportAbility(FVector DashDirection, FVector DashVelocity);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

