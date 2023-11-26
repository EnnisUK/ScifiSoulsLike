// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/MainCharacter.h"
#include "AIInterface.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "AbilitySystemComponent.h"
#include "Systems/SoulsLikeAttributeSet.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"




//////////////////////////////////////////////////////////////////////////
// AScifiSoulsLikeCharacter

AMainCharacter::AMainCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->MaxWalkSpeed = 700.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->GravityScale = 1.8;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	// Set Character Variables

	m_AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");
	m_AbilitySystemComponent->SetIsReplicated(true);
	m_AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	m_Attributes = CreateDefaultSubobject<USoulsLikeAttributeSet>("Attributes");



	m_WeaponChildClass = CreateDefaultSubobject<UChildActorComponent>("WeaponChildClass");
	m_WeaponChildClass->SetupAttachment(GetMesh());
}



void AMainCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (m_AbilitySystemComponent)
	{
		m_AbilitySystemComponent->InitAbilityActorInfo(this, this);

		InitializeAttributes();
		GiveDefaultAbilities();
	}
}

void AMainCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (m_AbilitySystemComponent)
	{
		m_AbilitySystemComponent->InitAbilityActorInfo(this, this);

		InitializeAttributes();
	}
}

void AMainCharacter::InitializeAttributes()
{
	if (m_AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = m_AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		FGameplayEffectSpecHandle SpecHandle = m_AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = m_AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void AMainCharacter::GiveDefaultAbilities()
{
	if (HasAuthority() && m_AbilitySystemComponent)
	{
		for (TSubclassOf<UGameplayAbility>& StartupAbility : DefaultAbilites)
			m_AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility.GetDefaultObject(), 1, 0));
	}
}

UAbilitySystemComponent* AMainCharacter::GetAbilitySystemComponent() const
{
	return m_AbilitySystemComponent;
}

void AMainCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	FTimerHandle CheckVelocityTimer;
	GetWorldTimerManager().SetTimer(CheckVelocityTimer, this, &AMainCharacter::CheckVelocity, 0.01, true);

	m_WeaponActor = UGameplayStatics::GetActorOfClass(GetWorld(), m_EquipWeaponClass);
	 m_WeaponRef = Cast<ABaseWeaponClass>(m_WeaponActor);
}



//////////////////////////////////////////////////////////////////////////
// Input

void AMainCharacter::CheckVelocity()
{
	float VectorLength = GetVelocity().Length();
	if (VectorLength < 200)
	{
		if (m_IsSprinting)
		{
			return;
		}
		

	}
}

void AMainCharacter::RollInput()
{
	
	
}

void AMainCharacter::RollAbility()
{
	
	
}

void AMainCharacter::RollCooldown()
{
	
}

void AMainCharacter::BasicAttack()
{
	if (m_IsGrounded)
	{
		if (!m_IsAttacking)
		{
			if (m_BasicAttackMontage)
			{
				m_IsAttacking = true;
				PlayAnimMontage(m_BasicAttackMontage);

				GetWorldTimerManager().SetTimer(m_WeaponTraceTimer, this, &AMainCharacter::WeaponLineTrace, 0.05, true);
			}
		
		}
	
	}
}

void AMainCharacter::WeaponLineTrace()
{
	if (m_IsAttacking)
	{

	
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		FHitResult Hit;

		if (m_WeaponRef)
		{
			float DamageAmount = m_WeaponRef->m_DamageAmount;

			if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), m_WeaponRef->m_StartSocket, m_WeaponRef->m_EndSocket, m_WeaponRef->m_WeaponRadius, m_ObjectType, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hit, true))
			{
				if (Hit.GetActor()->Implements<UAIInterface>())
				{
					IAIInterface::Execute_Damage(Hit.GetActor(), m_WeaponRef->m_DamageAmount);
				}

			}

		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Weapon Cast Failed"));
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(m_WeaponTraceTimer);
	}
}

void AMainCharacter::Block()
{
	if (m_IsGrounded)
	{
		
	}
}

void AMainCharacter::BlockFinish()
{
	

}

void AMainCharacter::Lockon()
{
	FHitResult Hit;
	if (!m_TargetLock)
	{
		
		

		FVector Start = GetActorLocation();
		FVector End = GetFollowCamera()->GetForwardVector() * m_TargetLockDistance + Start;
		
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);
		

		if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), Start, End, 300, m_ObjectType, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hit, true))
		{
			if (Hit.GetActor()->Implements<UAIInterface>())
			{
				IAIInterface::Execute_Lockon(Hit.GetActor());

				m_TargetLock = true;
				m_HitTarget = Hit.GetActor();
	
				

				GetWorldTimerManager().SetTimer(m_TrackEnemyTimer, this, &AMainCharacter::TrackEnemy, 0.001, true);
			}
		
		
		}
	}
	else
	{
		if (IsValid(m_HitTarget))
		{
			IAIInterface::Execute_EndLockon(m_HitTarget);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Black, TEXT("Hit Target Not Valid"));
		}
		
		m_TargetLock = false;
		GetWorldTimerManager().ClearTimer(m_TrackEnemyTimer);

		FTimerHandle ClearTargetTimer;
		GetWorldTimerManager().SetTimer(ClearTargetTimer, this, &AMainCharacter::ClearHitTarget, 0.1, false);

		

	}
}

void AMainCharacter::TrackEnemy()
{
	if (m_HitTarget)
	{
		

		FRotator EnemyRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), m_HitTarget->GetActorLocation());
		GetController()->SetControlRotation(EnemyRotation);
	}
}

void AMainCharacter::ClearHitTarget()
{
	m_HitTarget = NULL;
}

void AMainCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);

		//Sprinting
		//EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMainCharacter::StartSprint);

		//Roll
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &AMainCharacter::RollInput);

		//Basic Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMainCharacter::BasicAttack);

		//Lockon
		EnhancedInputComponent->BindAction(LockonAction, ETriggerEvent::Triggered, this, &AMainCharacter::Lockon);

		//Block
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Triggered, this, &AMainCharacter::Block);
		//BlockFinish
		EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &AMainCharacter::BlockFinish);

		

	}

}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMainCharacter::Look(const FInputActionValue& Value)
{
	if (!m_TargetLock)
	{
		// input is a Vector2D
		FVector2D LookAxisVector = Value.Get<FVector2D>();

		if (Controller != nullptr)
		{
			// add yaw and pitch input to controller
			AddControllerYawInput(LookAxisVector.X);
			AddControllerPitchInput(LookAxisVector.Y);
		}
	}

}










