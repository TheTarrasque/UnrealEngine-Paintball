// Fill out your copyright notice in the Description page of Project Settings.

#include "RTCube.h"
#include "Engine.h"

// Sets default values
ARTCube::ARTCube()
{
	ThisScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = ThisScene;

	ThisMesh = CreateDefaultSubobject<URuntimeMeshComponent>(TEXT("GeneratedMesh"));
	ThisMesh->SetupAttachment(RootComponent);

	// This will now set the material to the object at creation. This is the Dynamic camera position material
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Materials/m_Matrix.m_Matrix"));
	Material = MaterialAsset.Object;
}

void ARTCube::PostActorCreated()
{
	Super::PostActorCreated();
	GenerateMesh();
}

void ARTCube::PostLoad()
{
	Super::PostLoad();
	GenerateMesh();
}

void ARTCube::GenerateMesh()
{
	Vertices.Reset();
	Triangles.Reset();
	Normals.Reset();
	Tangents.Reset();
	UVs.Reset();
	Colors.Reset();

	int32 TriangleIndexCount = 0;
	FVector DefinedShape[8];
	FRuntimeMeshTangent TangentSetup;

	DefinedShape[0] = FVector(PMCubeRaduis.X, PMCubeRaduis.Y, PMCubeRaduis.Z); // Forward Top Right
	DefinedShape[1] = FVector(PMCubeRaduis.X, PMCubeRaduis.Y, -PMCubeRaduis.Z); // Forward Bottom Right
	DefinedShape[2] = FVector(PMCubeRaduis.X, -PMCubeRaduis.Y, PMCubeRaduis.Z); // Forward Top Left
	DefinedShape[3] = FVector(PMCubeRaduis.X, -PMCubeRaduis.Y, -PMCubeRaduis.Z); //Forward Bottom Left

	DefinedShape[4] = FVector(-PMCubeRaduis.X, -PMCubeRaduis.Y, PMCubeRaduis.Z); // Forward Top Right
	DefinedShape[5] = FVector(-PMCubeRaduis.X, -PMCubeRaduis.Y, -PMCubeRaduis.Z); // Forward Bottom Right
	DefinedShape[6] = FVector(-PMCubeRaduis.X, PMCubeRaduis.Y, PMCubeRaduis.Z); // Forward Top Left
	DefinedShape[7] = FVector(-PMCubeRaduis.X, PMCubeRaduis.Y, -PMCubeRaduis.Z); //Forward Bottom Left

	//front
	TangentSetup = FRuntimeMeshTangent(0.f, 1.f, 0.f);
	AddQuadMesh(DefinedShape[0], DefinedShape[1], DefinedShape[2], DefinedShape[3], TriangleIndexCount, TangentSetup);

	// left
	TangentSetup = FRuntimeMeshTangent(1.f, 0.f, 0.f);
	AddQuadMesh(DefinedShape[2], DefinedShape[3], DefinedShape[4], DefinedShape[5], TriangleIndexCount, TangentSetup);

	// Back
	TangentSetup = FRuntimeMeshTangent(0.f, -1.f, 0.f);
	AddQuadMesh(DefinedShape[4], DefinedShape[5], DefinedShape[6], DefinedShape[7], TriangleIndexCount, TangentSetup);

	//Right
	TangentSetup = FRuntimeMeshTangent(-1.f, 0.f, 0.f);
	AddQuadMesh(DefinedShape[6], DefinedShape[7], DefinedShape[0], DefinedShape[1], TriangleIndexCount, TangentSetup);

	//top
	TangentSetup = FRuntimeMeshTangent(0.f, 1.f, 0.f);
	AddQuadMesh(DefinedShape[6], DefinedShape[0], DefinedShape[4], DefinedShape[2], TriangleIndexCount, TangentSetup);

	// Bottom
	TangentSetup = FRuntimeMeshTangent(0.f, -1.f, 0.f);
	AddQuadMesh(DefinedShape[1], DefinedShape[7], DefinedShape[3], DefinedShape[5], TriangleIndexCount, TangentSetup);

	// time to add the object to the screen
	ThisMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
	// and to apply a material/texture to it
	ThisMesh->SetMaterial(0, Material);
}

void ARTCube::AddTriangleMesh(FVector TopRight, FVector BottomRight, FVector BottomLeft, int32 & TriIndex, FRuntimeMeshTangent Tangent)
{
	int32 Point1 = TriIndex++;
	int32 Point2 = TriIndex++;
	int32 Point3 = TriIndex++;

	Vertices.Add(TopRight);
	Vertices.Add(BottomRight);
	Vertices.Add(BottomLeft);

	Triangles.Add(Point1);
	Triangles.Add(Point2);
	Triangles.Add(Point3);

	FVector ThisNorm = FVector::CrossProduct(TopRight, BottomRight).GetSafeNormal();
	for (int i = 0; i < 3; i++)
	{
		Normals.Add(ThisNorm);
		Tangents.Add(Tangent);
		Colors.Add(FColor::Green);
	}
	UVs.Add(FVector2D(0.f, 1.f));//Top Left
	UVs.Add(FVector2D(0.f, 0.f));//Bottom Left
	UVs.Add(FVector2D(1.f, 0.f));//Bottom Right

}

void ARTCube::AddQuadMesh(FVector TopRight, FVector BottomRight, FVector TopLeft, FVector BottomLeft, int32 & TriIndex, FRuntimeMeshTangent Tangent)
{
	int32 Point1 = TriIndex++;
	int32 Point2 = TriIndex++;
	int32 Point3 = TriIndex++;
	int32 Point4 = TriIndex++;

	Vertices.Add(TopRight);
	Vertices.Add(BottomRight);
	Vertices.Add(TopLeft);
	Vertices.Add(BottomLeft);

	Triangles.Add(Point1);
	Triangles.Add(Point2);
	Triangles.Add(Point3);

	Triangles.Add(Point4);
	Triangles.Add(Point3);
	Triangles.Add(Point2);

	FVector ThisNorm = FVector::CrossProduct(TopLeft - BottomRight, TopLeft - TopRight).GetSafeNormal();
	for (int i = 0; i < 4; i++)
	{
		Normals.Add(ThisNorm);
		Tangents.Add(Tangent);
		Colors.Add(FColor::Green);
	}
	UVs.Add(FVector2D(0.f, 1.f));//Top Left
	UVs.Add(FVector2D(0.f, 0.f));//Bottom Left
	UVs.Add(FVector2D(1.f, 1.f));//Top Right
	UVs.Add(FVector2D(1.f, 0.f));//Bottom Right

}

#if WITH_EDITOR
void ARTCube::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName MemberPropertyChanged = (PropertyChangedEvent.MemberProperty ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None);

	if ((MemberPropertyChanged == GET_MEMBER_NAME_CHECKED(ARTCube, PMCubeRaduis)))
	{
		GenerateMesh();
	}
}
#endif