// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Characters/BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUDs/BaseHUD.h"
#include "Components/CapsuleComponent.h"
#include "AudioCaptureComponent.h"
#include "Interfaces/InteractInterface.h"
#include "Widgets/PauseWidget.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(GetMesh(), FName("head"));
	Camera->bUsePawnControlRotation = true;
	
	AudioCaptureComponent = CreateDefaultSubobject<UAudioCaptureComponent>("AudioCaptureComponent");
	AudioCaptureComponent->SetupAttachment(GetCapsuleComponent());
	AudioCaptureComponent->bEnableBusSends = false;
	AudioCaptureComponent->bEnableBaseSubmix = false;
	AudioCaptureComponent->bEnableSubmixSends = false;
	
	DeathVolume = 0.3f;
}

void ABaseCharacter::UpdateInputSensitivity()
{
	float MouseSensitivityMultiplier = 1.0f;
    
	if (IConsoleVariable* MouseCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("input.MouseSensitivity")))
		MouseSensitivityMultiplier = MouseCVar->GetFloat();
}

// Called when the game starts or when	spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerHUD = Cast<ABaseHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	
	SprintMeter = SprintTime;
	
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed; 
	
	// Fixed tick
	GetWorldTimerManager().SetTimer(SprintTimer, this, &ABaseCharacter::FixedTick, FixedTickTime, true);
	
	// Interact trace
	GetWorldTimerManager().SetTimer(InteractableItemNameTimer, this, &ABaseCharacter::LineTraceInteractItemName, ShownInteractItemNameTime, true);
	
	AudioCaptureComponent->OnAudioEnvelopeValue.AddDynamic(this, &ABaseCharacter::AudioEnvelopeValue);
	
	OnDeathRegistered.AddDynamic(this, &ABaseCharacter::isDead);
	
	// Camera rec time 
	StartCountUp();
}

void ABaseCharacter::AudioEnvelopeValue(float EnvelopeValue)
{
	MicVolume = EnvelopeValue * 100.0f;
	
	if (PlayerHUD)
	{
		PlayerHUD->SetMicVolumeValue(MicVolume);
	}
	
	if (bIsEquip)
	{
		if (MicVolume > DeathVolume)
		{
			isDead();
		}
		OnEquipCameraDelegate.Broadcast(bIsEquip);
	}
}

void ABaseCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();
	
	if (GetController())
	{
		AddMovementInput(GetActorForwardVector(), MoveVector.Y);
		AddMovementInput(GetActorRightVector(), MoveVector.X);
	}
}

void ABaseCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookDirection = Value.Get<FVector2D>();
	
	float SensitivityMultiplier = 1.0f;
	if (IConsoleVariable* CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("input.MouseSensitivity")))
		SensitivityMultiplier = CVar->GetFloat();
	
	if (GetController() != nullptr)
	{
		AddControllerYawInput(LookDirection.X * SensitivityMultiplier);
		AddControllerPitchInput(LookDirection.Y * SensitivityMultiplier);
	}
}

void ABaseCharacter::StartSprint()
{
	bIsSprinting = true;
	if (!bIsRecovering)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		
		OnSprintStateChanged.Broadcast(true);
	}
}

void ABaseCharacter::StopSprint()
{
	bIsSprinting = false;
	
	if (!bIsRecovering)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		
		OnSprintStateChanged.Broadcast(false);
	}
}

void ABaseCharacter::ToggleCrouch()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		if (!bIsCrouch)
		{
			bIsCrouch = true;
			Crouch();
		}
		else
		{
			bIsCrouch = false;
			UnCrouch();
		}
	}
}

void ABaseCharacter::UseItem()
{
	if (EquippedItem && EquippedItem->Implements<UInteractInterface>())
	{
		IInteractInterface::Execute_UseItem(EquippedItem);
		return;
	}
	
	if (InteractableActor && InteractableActor->Implements<UInteractInterface>())
	{
		IInteractInterface::Execute_UseItem(InteractableActor);
	}
}

void ABaseCharacter::isDead()
{
	if (!bIsDead)
	{
		bIsDead = true;
		UE_LOG(LogTemp, Warning, TEXT("Player is dead"));
		
		// Disable input
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			DisableInput(PC);
		}
		
		// Enable mesh physics
		GetMesh()->SetCollisionProfileName("Ragdoll");
		GetMesh()->SetSimulatePhysics(true);
	}
}

void ABaseCharacter::FixedTick()
{
	FVector Velocity = GetCharacterMovement()->Velocity;
	float Speed = FVector(Velocity.X, Velocity.Y, 0.0f).Size();
	
	if (Speed <= 1.0f)
	{
		MovementState = EMovementState::E_Idle;
	}
	else if (Speed <= 251.0f)
	{
		MovementState = EMovementState::E_Walk;
	}
	else if (Speed <= 501.0f)
	{
		MovementState = EMovementState::E_Sprint;
	}
	
	if (bIsSprinting && !bIsRecovering && GetVelocity().Length() > WalkSpeed)
	{
		if (SprintMeter > 0.0f)
		{
			SprintMeter = FMath::Max(SprintMeter - FixedTickTime, 0.0f);
			
			if (SprintMeter <= 0.0f)
			{
				bIsRecovering = true;
				
				GetCharacterMovement()->MaxWalkSpeed = RecoveringWalkSpeed;
			}
		}
	}
	else
	{
		SprintMeter = FMath::Min(SprintMeter + FixedTickTime, SprintTime);
		
		if (SprintMeter >= SprintTime)
		{
			bIsRecovering = false;
			
			GetCharacterMovement()->MaxWalkSpeed = bIsSprinting ? SprintSpeed : WalkSpeed;
			
			OnSprintStateChanged.Broadcast(bIsSprinting);
		}
	}
	OnSprintMeterUpdated.Broadcast(SprintMeter / SprintTime);
}

void ABaseCharacter::Interact()
{
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + Camera->GetForwardVector() * InteractDistance;
	
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_GameTraceChannel1, CollisionParams);
	
	if (Hit.bBlockingHit)
	{	
		InteractableActor = Hit.GetActor();
		if (InteractableActor && InteractableActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			IInteractInterface::Execute_Interact(InteractableActor, this);
		}
	}
}

void ABaseCharacter::LineTraceInteractItemName()
{
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + Camera->GetForwardVector() * InteractDistance;
    
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
    
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_GameTraceChannel1, CollisionParams);
	
	HoveredInteractActor = nullptr;
	if (PlayerHUD)
	{
		PlayerHUD->ShowInteractImage(false);
	}
    
	if (bHit && Hit.GetActor())
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			HoveredInteractActor = HitActor;
            
			if (PlayerHUD)
			{
				PlayerHUD->ShowInteractImage(true);
			}
			return;
		}
	}
}

void ABaseCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	GetWorldTimerManager().ClearTimer(SprintTimer);
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Move);
		
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseCharacter::Look);
		
		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ABaseCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABaseCharacter::StopSprint);
		
		// Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ABaseCharacter::ToggleCrouch);

		// Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ABaseCharacter::Interact);
		EnhancedInputComponent->BindAction(UseItemAction, ETriggerEvent::Started, this, &ABaseCharacter::UseItem);
		
		// Pause game
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ABaseCharacter::PauseGame);
	}

}

void ABaseCharacter::StartCountUp()
{
	FTimerHandle CountTimer;
	GetWorldTimerManager().SetTimer(CountTimer, [this]
	{
		if (bIsEquip)
		{
			CountUp = CountUp += 0.01f;
			OnCountTimeChangedDelegate.Broadcast(CountUp);
			UE_LOG(LogTemp, Warning, TEXT("Current count: %f"), CountUp);
		}	
	}, 0.01f, true);
}

void ABaseCharacter::PauseGame()
{
	if (PauseWidgetClass)
	{
		if (!PauseWidget || !PauseWidget->IsInViewport())
		{
			PauseWidget = CreateWidget<UPauseWidget>(GetWorld(), PauseWidgetClass);
            
			if (PauseWidget)
			{
				PauseWidget->OnPauseMenuClosed.AddDynamic(this, &ABaseCharacter::PauseWidgetClosed);
			}
		}
        
		if (PauseWidget && !bIsGamePaused)
		{
			bIsGamePaused = true;
			PauseWidget->AddToViewport();
		}
	}
}

void ABaseCharacter::PauseMenuClosed()
{
	bIsGamePaused = false;
	PauseWidget = nullptr;
}

