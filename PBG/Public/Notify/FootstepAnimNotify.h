// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "FootstepAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class PBG_API UFootstepAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
protected:
	
	void PlayFootstepSound(AActor* OwnerActor);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Footstep)
	TMap<UPhysicalMaterial*, USoundBase*> FootstepSounds;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Footstep)
	USoundBase* WaterFootstepSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Footstep)
	USoundBase* DeepWaterFootstepSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Footstep)
	UPhysicalMaterial* WaterMaterial;
	
	UFUNCTION()
	void FootStepWaterSplashSound(AActor* Owner, FVector SoundLocation, float WaterDepth, bool TraceCollisionHit);
};
