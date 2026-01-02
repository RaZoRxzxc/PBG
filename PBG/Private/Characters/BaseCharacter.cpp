// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Characters/BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUDs/BaseHUD.h"
#include "Interfaces/InteractInterface.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(GetMesh(), TEXT("head"));
	Camera->bUsePawnControlRotation = true;
	
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed; 
	
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SprintMeter = SprintTime;
	
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	
	GetWorldTimerManager().SetTimer(SprintTimer, this, &ABaseCharacter::FixedTick, SprintFixedTickTime, true);
	
	GetWorldTimerManager().SetTimer(InteractableItemNameTimer, this, &ABaseCharacter::LineTraceInteractItemName, ShownInteractItemNameTime, true);
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
	
	if (GetController() != nullptr)
	{
		AddControllerYawInput(LookDirection.X);
		AddControllerPitchInput(LookDirection.Y);
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

void ABaseCharacter::FixedTick()
{
	FVector Velocity = GetCharacterMovement()->Velocity;
	float Speed = FVector(Velocity.X, Velocity.Y, 0.0f).Size();
	
	if (Speed <= 1.0f)
	{
		MovementState = EMovementState::E_Idle;
	}
	else if (Speed <= 401.0f)
	{
		MovementState = EMovementState::E_Walk;
	}
	else if (Speed <= 801.0f)
	{
		MovementState = EMovementState::E_Sprint;
	}
	
	if (bIsSprinting && !bIsRecovering && GetVelocity().Length() > WalkSpeed)
	{
		if (SprintMeter > 0.0f)
		{
			SprintMeter = FMath::Max(SprintMeter - SprintFixedTickTime, 0.0f);
			
			if (SprintMeter <= 0.0f)
			{
				bIsRecovering = true;
				
				GetCharacterMovement()->MaxWalkSpeed = RecoveringWalkSpeed;
			}
		}
	}
	else
	{
		SprintMeter = FMath::Min(SprintMeter + SprintFixedTickTime, SprintTime);
		
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
	
	DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false ,2);
	
	if (Hit.bBlockingHit)
	{	
		InteractableActor = Hit.GetActor();
		if (InteractableActor && InteractableActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			IInteractInterface::Execute_Interact(InteractableActor, this);
			UE_LOG(LogTemp, Log, TEXT("Interact Character"));
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
    
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f, 0);
    
	ABaseHUD* HUD = Cast<ABaseHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	
	InteractableActor = nullptr;
	if (HUD)
	{
		HUD->HideInteractBlock();
	}
    
	if (bHit && Hit.GetActor())
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			InteractableActor = HitActor;
			FText ActorName = IInteractInterface::Execute_GetItemName(HitActor);
            
			if (HUD)
			{
				HUD->SetInteractText(ActorName);
				HUD->ShowInteractBlock();
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

// Called every frame
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
	}

}

