// Fill out your copyright notice in the Description page of Project Settings.

#include "RunTimeMeshGeometry.h"
#include "Engine.h"

// Sets default values
ARunTimeMeshGeometry::ARunTimeMeshGeometry()
{
	ThisScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = ThisScene;

	ThisMesh = CreateDefaultSubobject<URuntimeMeshComponent>(TEXT("GenerateMesh"));
	ThisMesh->SetupAttachment(RootComponent);

}



void ARunTimeMeshGeometry::PostActorCreated()
{
	Super::PostActorCreated();
	GenerateMesh();
}

void ARunTimeMeshGeometry::PostLoad()
{
	Super::PostLoad();
	GenerateMesh();
}

void ARunTimeMeshGeometry::GenerateMesh()
{
	/*
		The meat and potatoes sort of speak. Here I process the creation of the shape.
		I am still working on making it more efficient and easier to follow but I had to crunch
	*/
	//clear the variables of old data
	Vertices.Reset();
	Triangles.Reset();
	Normals.Reset();
	Tangents.Reset();
	Colors.Reset();
	UVs.Reset();

	int32 triangleIndexer = 0;
	FVector CubeVertices[32];
	FVector NormalVertices;
	FRuntimeMeshTangent TangentVertices;
	// Unreal uses and upper-left origin UV
	// X = Forward; Y = Right; Z = Up
	FVector InnerEdges = CubeRadius * InnerEdgeSize;
	FBox BoundingBox = FBox(-CubeRadius, CubeRadius);
	// Define the primary cube shape
	CubeVertices[0] = (FVector(CubeRadius.X, CubeRadius.Y, CubeRadius.Z)); //
	CubeVertices[1] = (FVector(CubeRadius.X, CubeRadius.Y, -CubeRadius.Z));
	CubeVertices[2] = (FVector(CubeRadius.X, -CubeRadius.Y, CubeRadius.Z));
	CubeVertices[3] = (FVector(CubeRadius.X, -CubeRadius.Y, -CubeRadius.Z));

	CubeVertices[4] = (FVector(-CubeRadius.X, -CubeRadius.Y, CubeRadius.Z));
	CubeVertices[5] = (FVector(-CubeRadius.X, -CubeRadius.Y, -CubeRadius.Z));
	CubeVertices[6] = (FVector(-CubeRadius.X, CubeRadius.Y, CubeRadius.Z));
	CubeVertices[7] = (FVector(-CubeRadius.X, CubeRadius.Y, -CubeRadius.Z));

	// Define the inner shapes, I know there is a pattern. Just need to find it and put this in a loop 
	// I feel like I could save lots of line of code here
	//inner front
	CubeVertices[8] = (FVector(CubeRadius.X, InnerEdges.Y, InnerEdges.Z)); //
	CubeVertices[9] = (FVector(CubeRadius.X, InnerEdges.Y, -InnerEdges.Z));
	CubeVertices[10] = (FVector(CubeRadius.X, -InnerEdges.Y, InnerEdges.Z));
	CubeVertices[11] = (FVector(CubeRadius.X, -InnerEdges.Y, -InnerEdges.Z));

	//inner Left
	CubeVertices[12] = (FVector(InnerEdges.X, -CubeRadius.Y, InnerEdges.Z));
	CubeVertices[13] = (FVector(InnerEdges.X, -CubeRadius.Y, -InnerEdges.Z));
	CubeVertices[14] = (FVector(-InnerEdges.X, -CubeRadius.Y, InnerEdges.Z));
	CubeVertices[15] = (FVector(-InnerEdges.X, -CubeRadius.Y, -InnerEdges.Z));

	//inner back
	CubeVertices[16] = (FVector(-CubeRadius.X, -InnerEdges.Y, InnerEdges.Z));
	CubeVertices[17] = (FVector(-CubeRadius.X, -InnerEdges.Y, -InnerEdges.Z));
	CubeVertices[18] = (FVector(-CubeRadius.X, InnerEdges.Y, InnerEdges.Z));
	CubeVertices[19] = (FVector(-CubeRadius.X, InnerEdges.Y, -InnerEdges.Z));
	//inner Right
	CubeVertices[20] = (FVector(-InnerEdges.X, CubeRadius.Y, InnerEdges.Z));
	CubeVertices[21] = (FVector(-InnerEdges.X, CubeRadius.Y, -InnerEdges.Z));
	CubeVertices[22] = (FVector(InnerEdges.X, CubeRadius.Y, InnerEdges.Z));
	CubeVertices[23] = (FVector(InnerEdges.X, CubeRadius.Y, -InnerEdges.Z));
	//inner Top
	CubeVertices[24] = (FVector(-InnerEdges.X, InnerEdges.Y, CubeRadius.Z));
	CubeVertices[25] = (FVector(InnerEdges.X, InnerEdges.Y, CubeRadius.Z));
	CubeVertices[26] = (FVector(-InnerEdges.X, -InnerEdges.Y, CubeRadius.Z));
	CubeVertices[27] = (FVector(InnerEdges.X, -InnerEdges.Y, CubeRadius.Z));
	//inner Bottom
	CubeVertices[28] = (FVector(InnerEdges.X, InnerEdges.Y, -CubeRadius.Z));
	CubeVertices[29] = (FVector(-InnerEdges.X, InnerEdges.Y, -CubeRadius.Z));
	CubeVertices[30] = (FVector(InnerEdges.X, -InnerEdges.Y, -CubeRadius.Z));
	CubeVertices[31] = (FVector(-InnerEdges.X, -InnerEdges.Y, -CubeRadius.Z));




	// with the vertices set it is time to give it a skin.
	// there is a pattern here that I messed up at some point. my UVs are off
	// Front (X): 0-1-2-3
//	NormalVertices = FVector(1, 0, 0);
	TangentVertices = FRuntimeMeshTangent(0.f, 1.f, 0.f);
	FVector sampledata = FVector::CrossProduct(CubeVertices[0] - CubeVertices[3], CubeVertices[2] - CubeVertices[3]).GetSafeNormal();
	//GEngine->AddOnScreenDebugMessage(-1, 300.f, FColor::Red, FString::Printf(TEXT("Normal should be 1,0,0: %f, %f, %f"), sampledata.X, sampledata.Y, sampledata.Z));
	AddQuadMesh(CubeVertices[0], CubeVertices[1], CubeVertices[8], CubeVertices[9], triangleIndexer, TangentVertices);  // Left Quad
	AddQuadMesh(CubeVertices[0], CubeVertices[8], CubeVertices[2], CubeVertices[10], triangleIndexer, TangentVertices); // Top Quad
	AddQuadMesh(CubeVertices[9], CubeVertices[1], CubeVertices[11], CubeVertices[3], triangleIndexer, TangentVertices); // Bottom Quad
	AddQuadMesh(CubeVertices[10], CubeVertices[11], CubeVertices[2], CubeVertices[3], triangleIndexer, TangentVertices);// Right Quad

	//Left (-Y): 2-3-4-5
	//NormalVertices = FVector(0, -1, 0);
	TangentVertices = FRuntimeMeshTangent(1.f, 0.f, 0.f);
	AddQuadMesh(CubeVertices[2], CubeVertices[3], CubeVertices[12], CubeVertices[13], triangleIndexer, TangentVertices); // Left Quad
	AddQuadMesh(CubeVertices[2], CubeVertices[12], CubeVertices[4], CubeVertices[14], triangleIndexer, TangentVertices); // Top Quad
	AddQuadMesh(CubeVertices[13], CubeVertices[3], CubeVertices[15], CubeVertices[5], triangleIndexer, TangentVertices); // Bottom Quad
	AddQuadMesh(CubeVertices[14], CubeVertices[15], CubeVertices[4], CubeVertices[5], triangleIndexer, TangentVertices); // Right Quad
	//Back (-X): 4-5-6-7
	//NormalVertices = FVector(-1, 0, 0);
	TangentVertices = FRuntimeMeshTangent(0.f, -1.f, 0.f);
	AddQuadMesh(CubeVertices[4], CubeVertices[5], CubeVertices[16], CubeVertices[17], triangleIndexer, TangentVertices); // Left Quad
	AddQuadMesh(CubeVertices[4], CubeVertices[16], CubeVertices[6], CubeVertices[18], triangleIndexer, TangentVertices); // Top Quad
	AddQuadMesh(CubeVertices[17], CubeVertices[5], CubeVertices[19], CubeVertices[7], triangleIndexer, TangentVertices); // Bottom Quad
	AddQuadMesh(CubeVertices[18], CubeVertices[19], CubeVertices[6], CubeVertices[7], triangleIndexer, TangentVertices); // Right Quad

	//Right (Y): 6-7-0-1
	//NormalVertices = FVector(0, 1, 0);
	TangentVertices = FRuntimeMeshTangent(-1.f, 0.f, 0.f);
	AddQuadMesh(CubeVertices[6], CubeVertices[7], CubeVertices[20], CubeVertices[21], triangleIndexer, TangentVertices); // Left Quad
	AddQuadMesh(CubeVertices[6], CubeVertices[20], CubeVertices[0], CubeVertices[22], triangleIndexer, TangentVertices); // Top Quad
	AddQuadMesh(CubeVertices[21], CubeVertices[7], CubeVertices[23], CubeVertices[1], triangleIndexer, TangentVertices); // Bottom Quad
	AddQuadMesh(CubeVertices[22], CubeVertices[23], CubeVertices[0], CubeVertices[1], triangleIndexer, TangentVertices); // Right Quad

	//Top (Z): 6-0-4-2
	//NormalVertices = FVector(0, 0, 1);
	TangentVertices = FRuntimeMeshTangent(0.f, 1.f, 0.f);
	AddQuadMesh(CubeVertices[6], CubeVertices[0], CubeVertices[24], CubeVertices[25], triangleIndexer, TangentVertices); // Left Quad
	AddQuadMesh(CubeVertices[6], CubeVertices[24], CubeVertices[4], CubeVertices[26], triangleIndexer, TangentVertices); // Top Quad
	AddQuadMesh(CubeVertices[25], CubeVertices[0], CubeVertices[27], CubeVertices[2], triangleIndexer, TangentVertices); // Bottom Quad
	AddQuadMesh(CubeVertices[26], CubeVertices[27], CubeVertices[4], CubeVertices[2], triangleIndexer, TangentVertices); // Right Quad

	//Bottom (-Z): 1-7-3-5
	//NormalVertices = FVector(0, 0, -1);
	TangentVertices = FRuntimeMeshTangent(0.f, -1.f, 0.f);
	AddQuadMesh(CubeVertices[1], CubeVertices[7], CubeVertices[28], CubeVertices[29], triangleIndexer, TangentVertices); // Left Quad
	AddQuadMesh(CubeVertices[1], CubeVertices[28], CubeVertices[3], CubeVertices[30], triangleIndexer, TangentVertices); // Top Quad
	AddQuadMesh(CubeVertices[29], CubeVertices[7], CubeVertices[31], CubeVertices[5], triangleIndexer, TangentVertices); // Bottom Quad
	AddQuadMesh(CubeVertices[30], CubeVertices[31], CubeVertices[3], CubeVertices[5], triangleIndexer, TangentVertices); // Right Quad

	//Inside stuff
	//there is definitely a patter here where I can reduce the number of parameters down to 4
	// and just use the first method I created. Will need to find it.
	// will need normals tangents and uvs



	// front Left Pillar
//	AddPillar(10, 20, 11, 20, 21, 11); //should be 20, 21, 10, 11
	AddQuadMesh(CubeVertices[12], CubeVertices[13], CubeVertices[10], CubeVertices[11], triangleIndexer);
	// Back Left Pillar
//	AddPillar(22, 12, 23, 12, 13, 23); //should be 12, 13, 22, 23
	AddQuadMesh(CubeVertices[16], CubeVertices[17], CubeVertices[14], CubeVertices[15], triangleIndexer);

	// Back Right Pillar
//	AddPillar(14, 16, 15, 16, 17, 15); //should be 16, 17, 14, 15
	AddQuadMesh(CubeVertices[20], CubeVertices[21], CubeVertices[18], CubeVertices[19], triangleIndexer);

	//front Right Pillar
//	AddPillar(8, 9, 18, 18, 9, 19); // should be 8, 9, 18, 19
	AddQuadMesh(CubeVertices[8], CubeVertices[9], CubeVertices[22], CubeVertices[23], triangleIndexer);

	// Bottom Front Pillar
//	AddPillar(9, 11, 30, 30, 28, 9); //should be 30, 28, 11, 9
	AddQuadMesh(CubeVertices[30], CubeVertices[28], CubeVertices[11], CubeVertices[9], triangleIndexer);

	// Bottom Left Pillar
//	AddPillar(21, 23, 31, 31, 30, 21); //should be 31, 30, 23, 21
	AddQuadMesh(CubeVertices[31], CubeVertices[30], CubeVertices[15], CubeVertices[13], triangleIndexer);

	// Bottom Back Pillar
//	AddPillar(13, 15, 29, 29, 31, 13); //should be 29, 31, 15, 13
	AddQuadMesh(CubeVertices[29], CubeVertices[31], CubeVertices[19], CubeVertices[17], triangleIndexer);

	// Bottom Right Pillar
//	AddPillar(17, 19, 28, 28, 29, 17); //should be 28, 29, 19, 17
	AddQuadMesh(CubeVertices[28], CubeVertices[29], CubeVertices[23], CubeVertices[21], triangleIndexer);

	//// Bottom Front Left Corner
	//AddTriangle(13, 30, 11);
	//NormalVertices = FVector(-1, 0, 0);
	TangentVertices = FRuntimeMeshTangent(0.f, -1.f, 0.f);
	AddTriangleMesh(CubeVertices[13], CubeVertices[30], CubeVertices[11], triangleIndexer, TangentVertices);

	//// Bottom Back Left Corner
	//AddTriangle(17, 31, 15);
	//NormalVertices = FVector(0, 1, 0);
	TangentVertices = FRuntimeMeshTangent(0.f, 1.f, 0.f);
	AddTriangleMesh(CubeVertices[17], CubeVertices[31], CubeVertices[15], triangleIndexer, TangentVertices);

	//// Bottom Back Right Corner
	//AddTriangle(21, 29, 19);
	//NormalVertices = FVector(-1, 1, 0);
	TangentVertices = FRuntimeMeshTangent(0.f, -1.f, 0.f);
	AddTriangleMesh(CubeVertices[21], CubeVertices[29], CubeVertices[19], triangleIndexer, TangentVertices);

	//// Bottom Front Right
	//AddTriangle(9, 28, 23);
	//NormalVertices = FVector(-1, -1, 0);
	TangentVertices = FRuntimeMeshTangent(0.f, -1.f, 0.f);
	AddTriangleMesh(CubeVertices[9], CubeVertices[28], CubeVertices[23], triangleIndexer, TangentVertices);

	// top corner sections. Man I took on too much
	// Top Front Pillar
//	AddPillar(25,27,10,10,8,25); //should be 10, 8, 25, 27
	AddQuadMesh(CubeVertices[10], CubeVertices[8], CubeVertices[27], CubeVertices[25], triangleIndexer);

	// Top Left Pillar
//	AddPillar(27,26,22,22,20,27); //should be 23, 20, 26, 27
	AddQuadMesh(CubeVertices[14], CubeVertices[12], CubeVertices[26], CubeVertices[27], triangleIndexer);

	//// Top Back Pillar
	//	AddPillar(26,24,14,14,12,26); //should be 14, 12, 24, 26
	AddQuadMesh(CubeVertices[18], CubeVertices[16], CubeVertices[24], CubeVertices[26], triangleIndexer);

	// Top Right Pillar
//	AddPillar(24,25,18,18,16,24); //should be 18, 16, 25, 24
	AddQuadMesh(CubeVertices[22], CubeVertices[20], CubeVertices[25], CubeVertices[24], triangleIndexer);

	// front Right Top Corner
	//AddTriangle(25, 8, 22);
	//NormalVertices = FVector(-1, 0, 0);
	TangentVertices = FRuntimeMeshTangent(0.f, -1.f, 0.f);
	AddTriangleMesh(CubeVertices[25], CubeVertices[8], CubeVertices[22], triangleIndexer, TangentVertices);

	//// Front left Top Corner
	//AddTriangle(27,12,10);
	//NormalVertices = FVector(1, -1, 0);
	TangentVertices = FRuntimeMeshTangent(0.f, -1.f, 0.f);
	AddTriangleMesh(CubeVertices[27], CubeVertices[12], CubeVertices[10], triangleIndexer, TangentVertices);
	//// back left Corner
	//AddTriangle(26, 16, 14);
	//NormalVertices = FVector(1, 1, 0);
	TangentVertices = FRuntimeMeshTangent(0.f, -1.f, 0.f);
	AddTriangleMesh(CubeVertices[26], CubeVertices[16], CubeVertices[14], triangleIndexer, TangentVertices);
	//// Back Right Corner
	//AddTriangle(24,20,18);
	//NormalVertices = FVector(1, -1, 0);
	TangentVertices = FRuntimeMeshTangent(0.f, -1.f, 0.f);
	AddTriangleMesh(CubeVertices[24], CubeVertices[20], CubeVertices[18], triangleIndexer, TangentVertices);
//
//	// with everything defined it is time to release it out in the world
	ThisMesh->ClearAllMeshSections();
	ThisMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
	
	// I made this property open in the editor so that materials can be added via that method. This allows for the ability to use the blockout material
	// to help with rapid prototyping and then to later apply a material that is more suitable for the game
	ThisMesh->SetMaterial(0, Material);
}
void ARunTimeMeshGeometry::AddQuadMesh(FVector TopLeft, FVector BottomLeft, FVector TopRight, FVector BottomRight, int32& TriIndex)
{
	int32 Triangle1 = TriIndex++;
	int32 Triangle2 = TriIndex++;
	int32 Triangle3 = TriIndex++;
	int32 Triangle4 = TriIndex++;
	Vertices.Add(TopLeft);
	Vertices.Add(BottomLeft);
	Vertices.Add(TopRight);
	Vertices.Add(BottomRight);
	AddTriangle(Triangle1, Triangle2, Triangle3, Triangle4);
}
void ARunTimeMeshGeometry::AddQuadMesh(FVector TopLeft, FVector BottomLeft, FVector TopRight, FVector BottomRight, int32& TriIndex, FRuntimeMeshTangent Tangent)
{
	int32 Triangle1 = TriIndex++;
	int32 Triangle2 = TriIndex++;
	int32 Triangle3 = TriIndex++;
	int32 Triangle4 = TriIndex++;
	Vertices.Add(TopLeft);
	Vertices.Add(BottomLeft);
	Vertices.Add(TopRight);
	Vertices.Add(BottomRight);

	AddTriangle(Triangle1, Triangle2, Triangle3, Triangle4);
	FVector ThisNorm = FVector::CrossProduct(TopLeft - BottomRight, TopLeft - TopRight).GetSafeNormal();
	for (int i = 0; i < 4; i++)
	{
		Normals.Add(ThisNorm);
		Tangents.Add(Tangent);
		Colors.Add(FColor::Green);
	}
	UVs.Add(FVector2D(0.0f, 0.0f)); //top left
	UVs.Add(FVector2D(0.0f, 1.0f)); //bottom left
	UVs.Add(FVector2D(1.0f, 0.0f)); //top right
	UVs.Add(FVector2D(1.0f, 1.0f)); //bottom right
}
void ARunTimeMeshGeometry::AddTriangleMesh(FVector Point1, FVector Point2, FVector Point3, int32 & TriIndex, FRuntimeMeshTangent Tangent)
{
	int32 Triangle1 = TriIndex++;
	int32 Triangle2 = TriIndex++;
	int32 Triangle3 = TriIndex++;
	Vertices.Add(Point1);
	Vertices.Add(Point2);
	Vertices.Add(Point3);

	AddTriangle(Triangle1, Triangle2, Triangle3);
	FVector ThisNorm = FVector::CrossProduct(Point1, Point2).GetSafeNormal();
	for (int i = 0; i < 3; i++)
	{
		Normals.Add(ThisNorm);
		Tangents.Add(Tangent);
		Colors.Add(FColor::Green);
	}
	UVs.Add(FVector2D(0.0f, 0.0f)); //top left
	UVs.Add(FVector2D(0.0f, 1.0f)); //bottom left
	UVs.Add(FVector2D(1.0f, 0.0f)); //top right
}

void ARunTimeMeshGeometry::AddTriangle(int32 p1, int32 p2, int32 p3)
{
	Triangles.Add(p1);
	Triangles.Add(p2);
	Triangles.Add(p3);
}

void ARunTimeMeshGeometry::AddTriangle(int32 A, int32 B, int32 C, int32 D)
{
	//Because the inner sections are so complicated, at this time anyway, I just man handled it and explicitly typed each triangle per face
	// again once I find a better pattern I will adjust the code
	//left panel
	Triangles.Add(A);
	Triangles.Add(B);
	Triangles.Add(D);

	Triangles.Add(A);
	Triangles.Add(D);
	Triangles.Add(C);

}

#if WITH_EDITOR

void ARunTimeMeshGeometry::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName MemberPropertyChanged = (PropertyChangedEvent.MemberProperty ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None);

	if ((MemberPropertyChanged == GET_MEMBER_NAME_CHECKED(ARunTimeMeshGeometry, CubeRadius))|| (MemberPropertyChanged == GET_MEMBER_NAME_CHECKED(ARunTimeMeshGeometry, InnerEdgeSize)))
	{
		GenerateMesh();
	}
	else if ((MemberPropertyChanged == GET_MEMBER_NAME_CHECKED(ARunTimeMeshGeometry, Material)))
	{
		ThisMesh->SetMaterial(0, Material);
	}
}
#endif // WITH_EDITOR
