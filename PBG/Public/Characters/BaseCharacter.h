// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "BaseCharacter.generated.h"

class UAudioCaptureComponent;

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	E_Idle UMETA(DisplayName = "Idle"),
	E_Walk UMETA(DisplayName = "Walk"),
	E_Sprint UMETA(DisplayName = "Sprint"),
};

UENUM(BlueprintType)
enum class EStandState : uint8
{
	E_Stand UMETA(DisplayName = "Stand"),
	E_Crouch UMETA(DisplayName = "Crouch")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathRegistered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateSprintMeter, float, Percentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSprintStateChanged, bool, bSprinting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipCameraChanged, bool, bEquip);
UCLASS()
class PBG_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EMovementState MovementState;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EStandState StandState;

protected:
	UPROPERTY()
	class ABaseHUD* PlayerHUD;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
	UAudioCaptureComponent* AudioCaptureComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float MicVolume;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	float DeathVolume;
	
private:
	bool bIsEquip = false;
	
public:
	bool GetIsEquip() const { return bIsEquip; }
	bool SetIsEquip(bool bIsEquipItem) { return bIsEquip = bIsEquipItem; }
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SprintAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* CrouchAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* UseItemAction;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void AudioEnvelopeValue(float EnvelopeValue);

private:
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	bool bIsSprinting = false;
	bool bIsRecovering = false;
	
	void StartSprint();
	void StopSprint();
	
	void ToggleCrouch();
	
	void UseItem();
protected:
	UFUNCTION()
	void isDead();
	bool bIsDead = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsCrouch = false;
	
private:
	/** Called while sprinting at a fixed time interval */
	void FixedTick();
	
	void Interact();
	void LineTraceInteractItemName();
	
	AActor* InteractableActor;
	AActor* HoveredInteractActor;
	
	FTimerHandle InteractableItemNameTimer;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	float ShownInteractItemNameTime = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	float InteractDistance = 500.0f;
	
	/** Time interval for sprinting stamina ticks */
	UPROPERTY(EditAnywhere, Category="Sprint", meta = (ClampMin = 0, ClampMax = 1, Units = "s"))
	float FixedTickTime = 0.03333f;

	/** Sprint stamina amount. Maxes at SprintTime */
	float SprintMeter = 0.0f;

	/** How long we can sprint for, in seconds */
	UPROPERTY(EditAnywhere, Category="Sprint", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float SprintTime = 10.0f;
	
	/** Walk speed while recovering stamina */
	UPROPERTY(EditAnywhere, Category="Recovery", meta = (ClampMin = 0, ClampMax = 10, Units = "cm/s"))
	float RecoveringWalkSpeed = 150.0f;

	/** Time it takes for the sprint meter to recover */
	UPROPERTY(EditAnywhere, Category="Recovery", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float RecoveryTime = 0.0f;

	/** Sprint tick timer */
	FTimerHandle SprintTimer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 250.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed = 500.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CrouchSpeed = 125.0f;
public:
	
	/** Delegate called when the sprint meter should be updated */
	FUpdateSprintMeter OnSprintMeterUpdated;

	/** Delegate called when we start and stop sprinting */
	FSprintStateChanged OnSprintStateChanged;
	
	FOnDeathRegistered OnDeathRegistered;
	
	FEquipCameraChanged OnEquipCameraDelegate;
	
protected:
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
