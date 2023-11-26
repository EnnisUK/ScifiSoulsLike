// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include <GameplayEffectTypes.h>
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "Components/ChildActorComponent.h"
#include "Systems/SoulsLikeAttributeSet.h"
#include "BaseWeaponClass.h"
#include "MainCharacter.generated.h"



UCLASS(config=Game)
class AMainCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	// Weapon Child Component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), meta = (DisplayName = "WeaponChildClass"))
	UChildActorComponent* m_WeaponChildClass;






	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	//Ability System
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilites, meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* m_AbilitySystemComponent;


	UPROPERTY()
	class USoulsLikeAttributeSet* m_Attributes;

	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	// Lockon Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LockonAction;

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
	class UInputAction* RollAction;

	// Melee Attack Call
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BlockAction;


public:
	AMainCharacter();

	// Overriden from IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;



	// Stamina Variables
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int m_MaxStamina = 100;


	bool m_IsSprinting;
	
	//Health Variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int m_MaxHealth = 100;

	// Energy Variables
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int m_MaxMana = 100;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int m_MaxStat = 99;


	//Teleport Ability Variables
	UPROPERTY(EditAnywhere)
	float m_RollDistance = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> m_EquipWeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Grounded"))
	bool m_IsGrounded;



	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool m_IFrames;

	bool m_RollCooldown;



	// Target Lock System

	bool m_TargetLock = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Lock Distance"))
	float m_TargetLockDistance = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ObjectType"))
	TArray <TEnumAsByte<EObjectTypeQuery>> m_ObjectType;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* m_HitTarget;

	// Attack Montages

	UPROPERTY(EditAnywhere)
	UAnimMontage* m_BasicAttackMontage;

	

	// Timer Handles For Character

	FTimerHandle m_DrainStaminaHandle;
	
	FTimerHandle m_DrainEnergyHandle;

	FTimerHandle m_RegenStaminaTimer;

	FTimerHandle m_TrackEnemyTimer;

	FTimerHandle m_WeaponTraceTimer;
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);




	// Functions For Stamina

	

	// Functions for Energy;
	
	void CheckVelocity();


	// Functions For Teleport/Dodge Ability
	void RollInput();

	void RollAbility();

	void RollCooldown();

	//Functions For Attacking Basic
	void BasicAttack();

	void WeaponLineTrace();

	void Block();

	void BlockFinish();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool m_IsAttacking;

	AActor* m_WeaponActor;

	ABaseWeaponClass* m_WeaponRef;




	// Functions for locking on

	void Lockon();

	void TrackEnemy();

	void ClearHitTarget();

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

	FORCEINLINE class UChildActorComponent* GetWeaponChild () const { return m_WeaponChildClass; }

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void InitializeAttributes();
	virtual void GiveDefaultAbilities();


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Abilites")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Abilites")
	TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilites;
};

