// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/**/
#include "NXL2019_lucianoProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DecalActor.h"
#include "Engine.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"

ANXL2019_lucianoProjectile::ANXL2019_lucianoProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ANXL2019_lucianoProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ANXL2019_lucianoProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL))
	{
		//Generate a 2D decal material into the world
		if (OtherComp->IsSimulatingPhysics())
		{
			Decals = UGameplayStatics::SpawnDecalAttached(SpawnMaterial, FVector(32.f, 64.f, 64.f),OtherComp, NAME_None , 
				Hit.Location, Hit.Normal.ToOrientationRotator(),EAttachLocation::KeepWorldPosition, 0.f);
			OtherComp->AddImpulseAtLocation(GetVelocity() * 500.0f, GetActorLocation());
		}
		else
		{
			Decals = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), SpawnMaterial, FVector(32.f, 64.f, 64.f), 
				Hit.Location, Hit.Normal.ToOrientationRotator(), 0.f);
		}
		if (Decals)
		{
			//Create the dynamic material instance and then make adjustments to the material
			SpawnMaterialInstance = Decals->CreateDynamicMaterialInstance();
			SpawnMaterialInstance->SetScalarParameterValue("Frame", UKismetMathLibrary::RandomIntegerInRange(0, 3));

			FLinearColor PaintColor;
			PaintColor = FLinearColor(UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), 
				UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), UKismetMathLibrary::RandomFloatInRange(0.f, 1.f));

			FString HitObjectName = OtherActor->GetName();
			if (HitObjectName.Contains(TEXT("Cube")))
			{
				PaintColor = FLinearColor(0.f, 1.f, 0.f);
			}
			SpawnMaterialInstance->SetVectorParameterValue("Color", PaintColor);

		}
		Destroy();
	}
}

