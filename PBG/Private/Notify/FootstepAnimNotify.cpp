// Fill out your copyright notice in the Description page of Project Settings.


#include "Notify/FootstepAnimNotify.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

void UFootstepAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp && MeshComp->GetOwner())
	{
		PlayFootstepSound(MeshComp->GetOwner());
	}
}

void UFootstepAnimNotify::PlayFootstepSound(AActor* OwnerActor)
{
	UWorld* World = OwnerActor->GetWorld();
	if (!World) return;
    
	FVector StartLoc = OwnerActor->GetActorLocation();
	FVector EndLoc = (OwnerActor->GetActorUpVector() * -200.0f) + StartLoc;
    
	FHitResult GroundHit;
	FHitResult WaterHit;
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(OwnerActor);
	CollisionParams.bReturnPhysicalMaterial = true;
    
	bool bInWater = World->LineTraceSingleByProfile(WaterHit, StartLoc, EndLoc, FName("WaterBodyCollision"), CollisionParams);
    
	if (bInWater && WaterHit.bBlockingHit)
	{
		if (WaterHit.PhysMaterial.Get() == WaterMaterial)
		{
			bool bGroundHit = World->LineTraceSingleByChannel(GroundHit,WaterHit.Location + FVector(0, 0, 10.0f),WaterHit.Location - FVector(0, 0, 200.0f),
				ECC_Visibility,CollisionParams);

			float Depth = 0.f;

			if (bGroundHit)
			{
				Depth = WaterHit.Location.Z - GroundHit.Location.Z;
			}
            	
			FootStepWaterSplashSound(OwnerActor, WaterHit.Location, Depth, bGroundHit);
			return;
		}
	}
	
	bool bHit = World->LineTraceSingleByChannel(GroundHit, StartLoc, EndLoc, ECollisionChannel::ECC_Visibility, CollisionParams);
    
	DrawDebugLine(World, StartLoc, EndLoc, bHit ? FColor::Red : FColor::Blue, false, 5.0f);
    
	if (bHit && GroundHit.bBlockingHit)
	{
		UPhysicalMaterial* Material = GroundHit.PhysMaterial.Get();
            
		if (Material && FootstepSounds.Contains(Material))
		{
			UGameplayStatics::PlaySoundAtLocation(World, FootstepSounds[Material], GroundHit.Location);
		}
	}
}

void UFootstepAnimNotify::FootStepWaterSplashSound(AActor* Owner, FVector SoundLocation, float WaterDepth,
	bool TraceCollisionHit)
{
	FVector SoundLoc = SoundLocation;
	bool bDeepWater = !TraceCollisionHit;
	
	if (bDeepWater)
	{
		UGameplayStatics::PlaySoundAtLocation(Owner, WaterFootstepSound, SoundLoc);
	}
	else
	{
		if (WaterDepth > 75.0f)
		{
			UGameplayStatics::PlaySoundAtLocation(Owner, DeepWaterFootstepSound, SoundLoc);
		}
		else if (WaterDepth > 35.0f)
		{
			UGameplayStatics::PlaySoundAtLocation(Owner, DeepWaterFootstepSound, SoundLoc);
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(Owner, WaterFootstepSound, SoundLoc);
		}
	}
}
