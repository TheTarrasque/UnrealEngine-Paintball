// Fill out your copyright notice in the Description page of Project Settings.

#include "Fog.h"
#include "Engine.h"

// Sets default values
AFog::AFog():m_WholeTextureRegion(0, 0, 0, 0, m_TextureSize, m_TextureSize)
{
	m_CoverSize = 500;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// Create a planar mesh from engine's planar static mesh
	m_SquarePlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Fog Plane Static Mesh"));
	RootComponent = m_SquarePlane;
	m_SquarePlane->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	m_SquarePlane->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName); {
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("/Engine/ArtTools/RenderToTexture/Meshes/S_1_Unit_Plane.S_1_Unit_Plane"));
		m_SquarePlane->SetStaticMesh(Asset.Object);
	}
	m_SquarePlane->TranslucencySortPriority = 100;

	m_SquarePlane->SetRelativeScale3D(FVector(m_CoverSize, m_CoverSize, 1));
	//still cant get the fog plan to rotate 90 degrees to be verticle. 

	m_SquarePlane->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// Load the base Material From your created material.
	{
		static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Textures/mFoW.mFoW"));
		m_DynamicMaterial = MaterialAsset.Object;
	}
	// Create the run-time fog texture
	if (!m_DynamicTexture)
	{
		m_DynamicTexture = UTexture2D::CreateTransient(m_TextureSize, m_TextureSize, PF_G8);
		m_DynamicTexture->CompressionSettings = TextureCompressionSettings::TC_Grayscale;
		m_DynamicTexture->SRGB = 0;
		m_DynamicTexture->UpdateResource();
		m_DynamicTexture->MipGenSettings = TMGS_NoMipmaps;
	}
	// Initialize array to all black
	for (int x = 0; x < m_TextureSize; ++x)
	{
		for (int y = 0; y < m_TextureSize; ++y) m_PixelArry[y * m_TextureSize + x] = 255;

		// Propagate memory's array to texture.
		if (m_DynamicTexture)
		{
			UpdateTextureRegions(m_DynamicTexture, 0, 1, &m_WholeTextureRegion, m_TextureSize, 1, m_PixelArry, false);
		}
	}
}

void AFog::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	//Create a dynamic Material instance to swap in the fog texture.
	if (m_DynamicMaterial)
	{
		m_DynamicMaterialInstance = UMaterialInstanceDynamic::Create(m_DynamicMaterial, this);
		m_DynamicMaterialInstance->SetTextureParameterValue("FogTexture", m_DynamicTexture);
	}

	// Set the dynamic material to the mesh.
	if (m_DynamicMaterialInstance) m_SquarePlane->SetMaterial(0, m_DynamicMaterialInstance);
}

void AFog::SetSize(float Value)
{
	m_CoverSize = Value;
	m_SquarePlane->SetRelativeScale3D(FVector(m_CoverSize, m_CoverSize, 1));
}

void AFog::RevealSmoothCircle(const FVector2D & pos, float radius)
{
	//// Calculate the where circle center is inside texture space
	//FVector2D Texel = pos;// -FVector2D(GetActorLocation().X, GetActorLocation().Y);
	//Texel = Texel * m_TextureSize / m_CoverSize;
	//Texel += FVector2D(m_TextureSize / 2, m_TextureSize / 2);

	//// Calculate radius in Texel unit (1 is 1 pixel)
	//float TexelRadius = radius * m_TextureSize / m_CoverSize;
	//// the square area to update
	//int MinX = FMath::Clamp<int>(Texel.X - TexelRadius, 0, m_TextureSize - 1);
	//int MinY = FMath::Clamp<int>(Texel.Y - TexelRadius, 0, m_TextureSize - 1);
	//int MaxX = FMath::Clamp<int>(Texel.X - TexelRadius, 0, m_TextureSize - 1);
	//int MaxY = FMath::Clamp<int>(Texel.Y - TexelRadius, 0, m_TextureSize - 1);
	//uint8 TheVal = 0;

	//// Update our array of fog value in memory
	//bool Dirty = false;
	//for (int x = MinX; x < MaxX; ++x)
	//{
	//	for (int y = MinY; y < MaxY; ++y)
	//	{
	//		float Distance = FVector2D::Distance(Texel, FVector2D(x, y));
	//		if (Distance < TexelRadius)
	//		{
	//			static float SmoothPct = 0.7f;
	//			uint8 OldVal = m_PixelArry[y * m_TextureSize + x];
	//			float lerp = FMath::GetMappedRangeValueClamped(FVector2D(SmoothPct, 1.0f), FVector2D(0, 1), Distance / TexelRadius);
	//			uint8 NewVal = lerp * 255;
	//			NewVal = FMath::Min(NewVal, OldVal);
	//			m_PixelArry[y * m_TextureSize + x] = NewVal;
	//			Dirty = Dirty || OldVal != NewVal;
	//		}
	//	}
	//}
	//// Propagate the values in memory's array to texture
	//if (Dirty) UpdateTextureRegions(m_DynamicTexture, 0, 1, &m_WholeTextureRegion, m_TextureSize, 1, m_PixelArry, false);
}

void AFog::UpdateTextureRegions(UTexture2D * Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D * Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
{
	if (Texture->Resource)
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
		};
		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;
		RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;

		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(UpdateTextureRegionData, FUpdateTextureRegionsData*, RegionData, RegionData, bool, bFreeData, bFreeData, {
			for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
			{
				int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
				if (RegionData->MipIndex >= CurrentFirstMip)
				{
					RHIUpdateTexture2D(RegionData->Texture2DResource->GetTexture2DRHI(), RegionData->MipIndex - CurrentFirstMip, RegionData->Regions[RegionIndex],
						RegionData->SrcPitch, RegionData->SrcData + RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch + RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp);
				}
			}
		if (bFreeData)
		{
			FMemory::Free(RegionData->Regions);
			FMemory::Free(RegionData->SrcData);
		}
		delete RegionData;
			});
	}
}

// Called when the game starts or when spawned
void AFog::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

