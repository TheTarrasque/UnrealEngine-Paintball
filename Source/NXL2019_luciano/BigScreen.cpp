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

//	ThisMesh->RelativeRotation = FRotator(0.f, -90.f, 0.f);
	ThisMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	ThisMesh->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
// cant figure this section of code out yet. I know what I want it to do but can get it to work
	//static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Materials/BigScreen_Materials/m_BigScreen.m_BigScreen"));
	//if (MaterialAsset.Succeeded())
	//{
	//	Material = MaterialAsset.Object;
	//}

//	//Create a dynamic Material instance to swap in the fog texture.
//	if (Material)
//	{
//		DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
////		m_DynamicMaterialInstance->SetTextureParameterValue("FogTexture", m_DynamicTexture);
//	}

	// Set the dynamic material to the mesh.
	//if (DynamicMaterial) ThisMesh->SetMaterial(0, DynamicMaterial);
	//DynamicMaterial = ThisMesh->CreateDynamicMaterialInstance();

}

void ABigScreen::BeginPlay()
{
	Super::BeginPlay();

}

