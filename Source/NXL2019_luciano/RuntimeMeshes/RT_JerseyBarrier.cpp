// Fill out your copyright notice in the Description page of Project Settings.

#include "RT_JerseyBarrier.h"


// Sets default values
ART_JerseyBarrier::ART_JerseyBarrier()
{
	ThisScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = ThisScene;

	ThisMesh = CreateDefaultSubobject<URuntimeMeshComponent>(TEXT("GenerateMesh"));
	ThisMesh->SetupAttachment(RootComponent);

	ThisCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	ThisCollision->InitBoxExtent(ShapeRadius * 1.1f);
	ThisCollision->SetupAttachment(RootComponent);
	ThisCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ThisCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	ThisCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// exploring setting the material at creation along with Runtime changing it
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Materials/m_Red.m_Red"));
	if (MaterialAsset.Succeeded())
	{
		MaterialA = MaterialAsset.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAssetB(TEXT("/Game/Materials/m_Blue.m_Blue"));
	if (MaterialAssetB.Succeeded())
	{
		MaterialB = MaterialAssetB.Object;
	}
}


void ART_JerseyBarrier::PostActorCreated()
{
	Super::PostActorCreated();
	GenerateMesh();
}

void ART_JerseyBarrier::PostLoad()
{
	Super::PostLoad();
	GenerateMesh();
}

void ART_JerseyBarrier::GenerateMesh()
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
	FVector CubeVertices[20];
	FVector NormalVertices;
	FRuntimeMeshTangent TangentVertices;
	// Unreal uses and upper-left origin UV
	// X = Forward; Y = Right; Z = Up

	// Front top Face
	CubeVertices[0] = (FVector(ShapeRadius.X * .25, ShapeRadius.Y,   JerseyBarrierHeight)); // Front Top Right
	CubeVertices[1] = (FVector(ShapeRadius.X * .25, ShapeRadius.Y,  -ShapeRadius.Z * .25)); // Front Middle Right
	CubeVertices[2] = (FVector(ShapeRadius.X * .25, ShapeRadius.Y, -ShapeRadius.Z)); // Front Bottom Right
	CubeVertices[3] = (FVector(ShapeRadius.X * .25, -ShapeRadius.Y, JerseyBarrierHeight)); // Front Top Left
	CubeVertices[4] = (FVector(ShapeRadius.X * .25, -ShapeRadius.Y, -ShapeRadius.Z  * .25)); // Front Middle Left
	CubeVertices[5] = (FVector(ShapeRadius.X * .25, -ShapeRadius.Y, -ShapeRadius.Z)); // Front Bottom Left
	// Front Bottom Face
	CubeVertices[6] = (FVector(ShapeRadius.X * .75,  ShapeRadius.Y,  -ShapeRadius.Z  * .5)); // Front Top Right
	CubeVertices[7] = (FVector(ShapeRadius.X     ,  ShapeRadius.Y, -ShapeRadius.Z)); // Front Bottom Right
	CubeVertices[8] = (FVector(ShapeRadius.X * .75, -ShapeRadius.Y,  -ShapeRadius.Z * .50)); // Front Top Left
	CubeVertices[9] = (FVector(ShapeRadius.X     , -ShapeRadius.Y, -ShapeRadius.Z)); // Front Bottom Left

	// Back top Face
	CubeVertices[10] = (FVector(-ShapeRadius.X * .25, -ShapeRadius.Y, JerseyBarrierHeight)); // Front Top Right
	CubeVertices[11] = (FVector(-ShapeRadius.X * .25, -ShapeRadius.Y, -ShapeRadius.Z * .25)); // Front Middle Right
	CubeVertices[12] = (FVector(-ShapeRadius.X * .25, -ShapeRadius.Y, -ShapeRadius.Z)); // Front Bottom Right
	CubeVertices[13] = (FVector(-ShapeRadius.X * .25, ShapeRadius.Y, JerseyBarrierHeight)); // Front Top Left
	CubeVertices[14] = (FVector(-ShapeRadius.X * .25, ShapeRadius.Y, -ShapeRadius.Z  * .25)); // Front Middle Left
	CubeVertices[15] = (FVector(-ShapeRadius.X * .25, ShapeRadius.Y, -ShapeRadius.Z)); // Front Bottom Left
	// Back Bottom Face
	CubeVertices[16] = (FVector(-ShapeRadius.X * .75, -ShapeRadius.Y, -ShapeRadius.Z  * .5)); // Front Top Right
	CubeVertices[17] = (FVector(-ShapeRadius.X      , -ShapeRadius.Y, -ShapeRadius.Z)); // Front Bottom Right
	CubeVertices[18] = (FVector(-ShapeRadius.X * .75, ShapeRadius.Y, -ShapeRadius.Z * .50)); // Front Top Left
	CubeVertices[19] = (FVector(-ShapeRadius.X      , ShapeRadius.Y, -ShapeRadius.Z)); // Front Bottom Left



	// with the vertices set it is time to give it a skin.
	// there is a pattern here that I messed up at some point. my UVs are off
	// Front (X): 0-1-2-3
	TangentVertices = FRuntimeMeshTangent(0.f, 1.f, 0.f);
	AddQuadMesh(CubeVertices[0], CubeVertices[1], CubeVertices[3], CubeVertices[4], triangleIndexer, TangentVertices);
	AddQuadMesh(CubeVertices[1], CubeVertices[6], CubeVertices[4], CubeVertices[8], triangleIndexer, TangentVertices);
	AddQuadMesh(CubeVertices[6], CubeVertices[7], CubeVertices[8], CubeVertices[9], triangleIndexer, TangentVertices);
	//top
	AddQuadMesh(CubeVertices[13], CubeVertices[0], CubeVertices[10], CubeVertices[3], triangleIndexer, TangentVertices);

	TangentVertices = FRuntimeMeshTangent(1.f, 0.f, 0.f);
	AddQuadMesh(CubeVertices[8], CubeVertices[9], CubeVertices[4], CubeVertices[5], triangleIndexer, TangentVertices);
	AddQuadMesh(CubeVertices[3], CubeVertices[4], CubeVertices[10], CubeVertices[11], triangleIndexer, TangentVertices);
	AddQuadMesh(CubeVertices[4], CubeVertices[5], CubeVertices[11], CubeVertices[12], triangleIndexer, TangentVertices);

	TangentVertices = FRuntimeMeshTangent(-1.f, 0.f, 0.f);
	AddQuadMesh(CubeVertices[1], CubeVertices[2], CubeVertices[6], CubeVertices[7], triangleIndexer, TangentVertices);
	AddQuadMesh(CubeVertices[13], CubeVertices[14], CubeVertices[0], CubeVertices[1], triangleIndexer, TangentVertices);
	AddQuadMesh(CubeVertices[14], CubeVertices[15], CubeVertices[1], CubeVertices[2], triangleIndexer, TangentVertices);

	TangentVertices = FRuntimeMeshTangent(0.f, -1.f, 0.f);
	AddQuadMesh(CubeVertices[7], CubeVertices[2], CubeVertices[9], CubeVertices[5], triangleIndexer, TangentVertices);
	AddQuadMesh(CubeVertices[2], CubeVertices[15], CubeVertices[5], CubeVertices[12], triangleIndexer, TangentVertices);

	// Back (X): 0-1-2-3
	TangentVertices = FRuntimeMeshTangent(0.f, -1.f, 0.f);
	AddQuadMesh(CubeVertices[10], CubeVertices[11], CubeVertices[13], CubeVertices[14], triangleIndexer, TangentVertices);
	AddQuadMesh(CubeVertices[11], CubeVertices[16], CubeVertices[14], CubeVertices[18], triangleIndexer, TangentVertices);
	AddQuadMesh(CubeVertices[16], CubeVertices[17], CubeVertices[18], CubeVertices[19], triangleIndexer, TangentVertices);

	TangentVertices = FRuntimeMeshTangent(-1.f, 0.f, 0.f);
	AddQuadMesh(CubeVertices[18], CubeVertices[19], CubeVertices[14], CubeVertices[15], triangleIndexer, TangentVertices);

	TangentVertices = FRuntimeMeshTangent(1.f, 0.f, 0.f);
	AddQuadMesh(CubeVertices[11], CubeVertices[12], CubeVertices[16], CubeVertices[17], triangleIndexer, TangentVertices);

	TangentVertices = FRuntimeMeshTangent(0.f, 1.f, 0.f);
	AddQuadMesh(CubeVertices[15], CubeVertices[19], CubeVertices[12], CubeVertices[17], triangleIndexer, TangentVertices);

	// now that the object has been defined and setup it is time to display our 3D object onto the screen within the game
	ThisMesh->ClearAllMeshSections();
	ThisMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
	// apply the base material/texture to the object
	ThisMesh->SetMaterial(0, MaterialA);
}
void ART_JerseyBarrier::AddQuadMesh(FVector TopLeft, FVector BottomLeft, FVector TopRight, FVector BottomRight, int32& TriIndex)
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
void ART_JerseyBarrier::AddQuadMesh(FVector TopLeft, FVector BottomLeft, FVector TopRight, FVector BottomRight, int32& TriIndex, FRuntimeMeshTangent Tangent)
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
void ART_JerseyBarrier::AddTriangleMesh(FVector Point1, FVector Point2, FVector Point3, int32 & TriIndex, FRuntimeMeshTangent Tangent)
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

void ART_JerseyBarrier::AddTriangle(int32 p1, int32 p2, int32 p3)
{
	Triangles.Add(p1);
	Triangles.Add(p2);
	Triangles.Add(p3);
}

void ART_JerseyBarrier::AddTriangle(int32 A, int32 B, int32 C, int32 D)
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

void ART_JerseyBarrier::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Color should change but crashes"));
	//JerseyBarrierHeight = 1000;
	//GenerateMesh();
	// apply the secondary material/texture to the object
	ThisMesh->SetMaterial(0, MaterialB);
}

void ART_JerseyBarrier::NotifyActorEndOverlap(AActor * OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Color should change but crashes"));
	//JerseyBarrierHeight = 100;
	//GenerateMesh();
	// apply the base material/texture to the object
	ThisMesh->SetMaterial(0, MaterialA);
}

void ART_JerseyBarrier::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Create a dynamic Material instance to swap in the fog texture.
	if (MaterialA)
	{
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(MaterialA, this);
	}

	// Set the dynamic material to the mesh.
	if (DynamicMaterialInstance) ThisMesh->SetMaterial(0, DynamicMaterialInstance);
}
#if WITH_EDITOR

void ART_JerseyBarrier::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName MemberPropertyChanged = (PropertyChangedEvent.MemberProperty ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None);

	if ((MemberPropertyChanged == GET_MEMBER_NAME_CHECKED(ART_JerseyBarrier, JerseyBarrierHeight)))
	{
		GenerateMesh();
	}
	else if ((MemberPropertyChanged == GET_MEMBER_NAME_CHECKED(ART_JerseyBarrier, MaterialA)))
	{
		ThisMesh->SetMaterial(0, MaterialA);
	}
}
#endif // WITH_EDITOR
