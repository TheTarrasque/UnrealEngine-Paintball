// Fill out your copyright notice in the Description page of Project Settings.

#include "BigScreen.h"
#include "Engine.h"


// Sets default values
ABigScreen::ABigScreen()
{
	ThisScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = ThisScene;

	ThisMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GeneratedMesh"));
	ThisMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("/Engine/ArtTools/RenderToTexture/Meshes/S_1_Unit_Plane.S_1_Unit_Plane"));
	if (Asset.Succeeded())
	{
		ThisMesh->SetStaticMesh(Asset.Object);
	}
	ThisMesh->SetRelativeScale3D(FVector(512, 512, 1));
	//set the rotation of the plane verticle

	ThisMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	ThisMesh->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
}

void ABigScreen::BeginPlay()
{
	Super::BeginPlay();

}

