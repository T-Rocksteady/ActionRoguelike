// Copyright Tetyana Meleshchenko. All rights reserved.

#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include "DrawDebugHelpers.h"

USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();
	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	const FVector End = EyeLocation + (EyeRotation.Vector() * 1000.f);
	
// 	FHitResult Hit;
// 	const bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	const float InteractionRadius = 30.f;
	FCollisionShape Shape;
	Shape.SetSphere(InteractionRadius);

	TArray<FHitResult> Hits;
	const bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);
	
	const FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	for (FHitResult Hit : Hits)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				if (APawn* MyPawn = Cast<APawn>(MyOwner))
				{
					ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
					break;
				}
			}
		}
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, InteractionRadius, 32, LineColor, false, 2.f);
	}

	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 2.f, 0u, 2.f);
}

void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}