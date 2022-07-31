// Fill out your copyright notice in the Description page of Project Settings.

#include "SExplosiveBarrel.h"
#include "PhysicsEngine/RadialForceComponent.h"

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
	Destroy();
}

void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StaticMesh->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnHitCallback);
}