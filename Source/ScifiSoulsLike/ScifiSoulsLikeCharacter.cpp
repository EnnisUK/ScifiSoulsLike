// Copyright Epic Games, Inc. All Rights Reserved.

#include "ScifiSoulsLikeCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


//////////////////////////////////////////////////////////////////////////
// AScifiSoulsLikeCharacter

AScifiSoulsLikeCharacter::AScifiSoulsLikeCharacter()
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
	GetCharacterMovement()->JumpZVelocity = 700.f;
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

	m_Health = m_MaxHealth;
	m_Stamina = m_MaxStamina;
	m_Energy = m_MaxEnergy;
}

void AScifiSoulsLikeCharacter::BeginPlay()
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
}

//////////////////////////////////////////////////////////////////////////
// Input

void AScifiSoulsLikeCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AScifiSoulsLikeCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AScifiSoulsLikeCharacter::Look);

		//Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AScifiSoulsLikeCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AScifiSoulsLikeCharacter::EndSprint);

	}

}

void AScifiSoulsLikeCharacter::Move(const FInputActionValue& Value)
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

void AScifiSoulsLikeCharacter::Look(const FInputActionValue& Value)
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

void AScifiSoulsLikeCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 1200.f;
	
	GetWorldTimerManager().SetTimer(m_DrainStaminaHandle, this, &AScifiSoulsLikeCharacter::DrainStamina, 0.1, true);

	m_IsSprinting = true;
}

void AScifiSoulsLikeCharacter::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 700.f;
	m_IsSprinting = false;

	FTimerHandle StartRegenTimer;
	GetWorldTimerManager().SetTimer(StartRegenTimer, this, &AScifiSoulsLikeCharacter::RegenStamina, 1.5, false);
}

void AScifiSoulsLikeCharacter::DrainStamina()
{
	if (m_Stamina != 0 && m_IsSprinting == true)
	{
		m_Stamina -= 1.0f;
		m_Stamina = FMath::Clamp(m_Stamina, 0.0f, m_MaxStamina);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(m_DrainStaminaHandle);
		EndSprint();
	}
}

void AScifiSoulsLikeCharacter::RegenStamina()
{
	FTimerHandle RegenStaminaTimer;

	if (m_Stamina != 100 && m_IsSprinting == false)
	{
		m_Stamina += 1.0f;
		m_Stamina = FMath::Clamp(m_Stamina, 0.0f, m_MaxStamina);
		
		GetWorldTimerManager().SetTimer(RegenStaminaTimer, this, &AScifiSoulsLikeCharacter::RegenStamina, 0.1, true);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(RegenStaminaTimer);
	}
}




