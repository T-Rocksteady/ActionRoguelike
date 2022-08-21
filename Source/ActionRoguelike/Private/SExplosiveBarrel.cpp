// Copyright Tetyana Meleshchenko. All rights reserved.

#include "SExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "DrawDebugHelpers.h"

ASExplosiveBarrel::ASExplosiveBarrel()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	RootComponent = StaticMesh;

	RadialForce = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForce->SetupAttachment(StaticMesh);
	RadialForce->SetAutoActivate(false);
	RadialForce->Radius = 750.f;
	RadialForce->ImpulseStrength = 2500.f;
	RadialForce->bImpulseVelChange = true;
}

void ASExplosiveBarrel::OnHitCallback(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForce->FireImpulse();

	UE_LOG(LogTemp, Warning, TEXT("Other Actor: %s, at game time: %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);
	const FString CombinedString = FString::Printf(TEXT("Hit at location: %s"), *Hit.ImpactPoint.ToString());
	DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.f, true);

	Destroy();
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StaticMesh->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnHitCallback);
}