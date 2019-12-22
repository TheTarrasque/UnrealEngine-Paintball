// Fill out your copyright notice in the Description page of Project Settings.

#include "PMMeshGenerator.h"
#include "Engine.h"

// Sets default values
APMMeshGenerator::APMMeshGenerator()
{
	ThisScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = ThisScene;

	ThisMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	ThisMesh->SetupAttachment(RootComponent);
}

void APMMeshGenerator::PostActorCreated()
{
	Super::PostActorCreated();
	GenerateMesh();
}

void APMMeshGenerator::PostLoad()
{
	Super::PostLoad();
	GenerateMesh();
}

void APMMeshGenerator::GenerateMesh()
{
	// These will ensure the shape is completely refreshed on each change
	Vertices.Reset();
	Triangles.Reset();
	Normals.Reset();
	Tangents.Reset();
	UVs.Reset();
	Colors.Reset();

	int32 TriangleIndexCount = 0;
	FVector DefinedShape[4];
	FProcMeshTangent TangentSetup;

	DefinedShape[0] = FVector(QuadRadius.X, QuadRadius.Y, QuadRadius.Z); // Forward Top Right
	DefinedShape[1] = FVector(QuadRadius.X, QuadRadius.Y, -QuadRadius.Z); // Forward Bottom Right
	DefinedShape[2] = FVector(QuadRadius.X, -QuadRadius.Y, QuadRadius.Z); // Forward Top Left
	DefinedShape[3] = FVector(QuadRadius.X, -QuadRadius.Y, -QuadRadius.Z); //Forward Bottom Left

	TangentSetup = FProcMeshTangent(0.f, 1.f, 0.f);
	AddQuadMesh(DefinedShape[0], DefinedShape[1], DefinedShape[2], DefinedShape[3], TriangleIndexCount, TangentSetup);

	ThisMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);

}

void APMMeshGenerator::AddTriangleMesh(FVector TopRight, FVector BottomRight, FVector BottomLeft, int32 & TriIndex, FProcMeshTangent Tangent)
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
		Colors.Add(FLinearColor::Green);
	}
	UVs.Add(FVector2D(0.f, 1.f));//Top Left
	UVs.Add(FVector2D(0.f, 0.f));//Bottom Left
	UVs.Add(FVector2D(1.f, 0.f));//Bottom Right

}

void APMMeshGenerator::AddQuadMesh(FVector TopRight, FVector BottomRight, FVector TopLeft, FVector BottomLeft, int32 & TriIndex, FProcMeshTangent Tangent)
{
	// used to increment the Triangle index counter along with 
	// making it more readable when ordering the trianglebuffer
	int32 Point1 = TriIndex++;
	int32 Point2 = TriIndex++;
	int32 Point3 = TriIndex++;
	int32 Point4 = TriIndex++;

	//Will add the vertices needed in the correct order
	Vertices.Add(TopRight);
	Vertices.Add(BottomRight);
	Vertices.Add(TopLeft);
	Vertices.Add(BottomLeft);

	//Note: Order matters
	//if the quad surface is FACING YOU then go counter clock wise
	//if the quad surface is facing the same direction as you then go clock wise
	Triangles.Add(Point1);//Your Top Left
	Triangles.Add(Point2);//Your Bottom Left
	Triangles.Add(Point3);//Your Top Right

	Triangles.Add(Point4);//Your Bottom Right
	Triangles.Add(Point3);//Your Top Right
	Triangles.Add(Point2);//Your Bottom Left

	//This took a bit to understand but I am glad I took the time to figure it out
	FVector ThisNorm = FVector::CrossProduct(TopLeft - BottomRight, TopLeft - TopRight).GetSafeNormal();
	for (int i = 0; i < 4; i++)
	{
		Normals.Add(ThisNorm);
		Tangents.Add(Tangent);
		Colors.Add(FLinearColor::Green);
	}
	// I know there is an easier way but this is the best way I know for now
	UVs.Add(FVector2D(0.f, 1.f));//Top Left
	UVs.Add(FVector2D(0.f, 0.f));//Bottom Left
	UVs.Add(FVector2D(1.f, 1.f));//Top Right
	UVs.Add(FVector2D(1.f, 0.f));//Bottom Right

}

#if WITH_EDITOR
void APMMeshGenerator::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	// Get the name of the property that was changed
	FName MemberPropertyChanged = (PropertyChangedEvent.MemberProperty ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None);

	// now check if it is a property we care about. if so trigger a method. in this case the GenerateMesh is triggered
	if ((MemberPropertyChanged == GET_MEMBER_NAME_CHECKED(APMMeshGenerator, QuadRadius)))
	{
		GenerateMesh();
	}
}
#endif