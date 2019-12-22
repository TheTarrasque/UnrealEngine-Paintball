// Fill out your copyright notice in the Description page of Project Settings.
// Tutorial creator Phung Dinh Thang, Thanks!

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Fog.generated.h"

UCLASS()
class NXL2019_LUCIANO_API AFog : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFog();


	virtual void PostInitializeComponents() override;

	// Set the fog size
	void SetSize(float Value);

	// Reveal a portion of the fog
	void RevealSmoothCircle(const FVector2D& pos, float radius);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions,
		FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData);

	// Fog texture size
	static const int m_TextureSize = 512;

	UPROPERTY() UStaticMeshComponent* m_SquarePlane;
	UPROPERTY() UTexture2D* m_DynamicTexture;
	UPROPERTY() UMaterialInterface* m_DynamicMaterial;
	UPROPERTY() UMaterialInstanceDynamic* m_DynamicMaterialInstance;

	uint8 m_PixelArry[m_TextureSize * m_TextureSize];
	FUpdateTextureRegion2D m_WholeTextureRegion;
	float m_CoverSize;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
