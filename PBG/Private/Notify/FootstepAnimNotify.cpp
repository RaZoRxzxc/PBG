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
    
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(OwnerActor);
	CollisionParams.bTraceComplex = true;
	CollisionParams.bReturnPhysicalMaterial = true;
    
	bool bHit = World->LineTraceSingleByChannel(Hit, StartLoc, EndLoc, ECollisionChannel::ECC_Visibility, CollisionParams);
    
	DrawDebugLine(World, StartLoc, EndLoc, bHit ? FColor::Red : FColor::Blue, false, 5.0f);
    
	if (bHit && Hit.bBlockingHit)
	{
		UPhysicalMaterial* Material = Hit.PhysMaterial.Get();
            
		if (Material && FootstepSounds.Contains(Material))
		{
			UGameplayStatics::PlaySoundAtLocation(World, FootstepSounds[Material], Hit.Location);
			UE_LOG(LogTemp, Warning, TEXT("Footstep sound"));
		}
	}
}
