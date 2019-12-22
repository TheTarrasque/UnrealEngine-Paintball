// Fill out your copyright notice in the Description page of Project Settings.

#include "PM_Paramid.h"
#include "Engine.h"

// Sets default values
APM_Paramid::APM_Paramid()
{
	ThisScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = ThisScene;

	ThisMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GenerateMesh"));
	ThisMesh->SetupAttachment(RootComponent);

	ThisTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	ThisTrigger->InitBoxExtent(ShapeHeight * .85f);
	ThisTrigger->SetupAttachment(RootComponent);
	ThisTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ThisTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	ThisTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}
void APM_Paramid::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	PillarRise = 1000;
	FTimerHandle NotUsedHandle;
	GetWorldTimerManager().SetTimer(NotUsedHandle, this, &APM_Paramid::GenerateMesh, 5.f, false);
//	GenerateMesh();
	
}

void APM_Paramid::NotifyActorEndOverlap(AActor * OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	PillarRise = 0;
	FTimerHandle NotUsedHandle;
	GetWorldTimerManager().SetTimer(NotUsedHandle, this, &APM_Paramid::GenerateMesh, 5.f, false);
//	GenerateMesh();

}
// This method is called anytime an actor is spawned. whether during runtime or being placed in the world in the editor
void APM_Paramid::PostActorCreated()
{
	Super::PostActorCreated();
	GenerateMesh();

}

//this method is called anytime a level is opened
void APM_Paramid::PostLoad()
{
	Super::PostLoad();
	GenerateMesh();

}

void APM_Paramid::GenerateMesh()
{
	/*
		The meat and potatoes sort of speak. Here I process the creation of the shape.
		I am still working on making it more effecient and easier to follow but I had to crunch
	*/
	//clear the variables of old data
	Vertices.Reset();
	Triangles.Reset();
	Normals.Reset();
	Tangents.Reset();
	Colors.Reset();
	UVs.Reset();

	int32 triangleIndexer = 0;
	FVector CubeVertices[16];
	FVector NormalVertices;
	FProcMeshTangent TangentVertices;
	// Unreal uses and upper-left origin UV
	// X = Forward; Y = Right; Z = Up
	FVector InnerRing1 = ShapeHeight * .9f;
	FVector InnerRing2 = ShapeHeight * .85f;
	InnerRing2.Z += PillarRise;
	// Front
	CubeVertices[0] = (FVector(ShapeHeight.X,  ShapeHeight.Y,  ShapeHeight.Z)); //Front Right Top
	CubeVertices[1] = (FVector(ShapeBase.X,    ShapeBase.Y,    -ShapeBase.Z)); //Front Right Bottom
	CubeVertices[2] = (FVector(ShapeHeight.X, -ShapeHeight.Y, ShapeHeight.Z)); //Front Left Top
	CubeVertices[3] = (FVector(ShapeBase.X,   -ShapeBase.Y,   -ShapeBase.Z)); //Front Left Bottom
	//Back										  				 
	CubeVertices[4] = (FVector(-ShapeHeight.X, -ShapeHeight.Y,  ShapeHeight.Z));  //Back Right Top
	CubeVertices[5] = (FVector(-ShapeBase.X,   -ShapeBase.Y,    -ShapeBase.Z)); //Back Right Bottom
	CubeVertices[6] = (FVector(-ShapeHeight.X,  ShapeHeight.Y, ShapeHeight.Z)); //Back Left Top
	CubeVertices[7] = (FVector(-ShapeBase.X,    ShapeBase.Y,   -ShapeBase.Z));//Back Left Bottom
	//top Inner ring
	CubeVertices[8] =  (FVector(-InnerRing1.X,  InnerRing1.Y, InnerRing1.Z));  //Back Right Top
	CubeVertices[9] =  (FVector( InnerRing1.X,  InnerRing1.Y, InnerRing1.Z)); //Front Right Top
	CubeVertices[10] = (FVector(-InnerRing1.X, -InnerRing1.Y, InnerRing1.Z)); //Back Left Top
	CubeVertices[11] = (FVector( InnerRing1.X, -InnerRing1.Y, InnerRing1.Z));//Front Left Top

	//top Inner inner ring
	CubeVertices[12] =  (FVector(-InnerRing2.X,  InnerRing2.Y, InnerRing2.Z));  //Back Right Top
	CubeVertices[13] =  (FVector( InnerRing2.X,  InnerRing2.Y, InnerRing2.Z)); //Front Right Top
	CubeVertices[14] = (FVector(-InnerRing2.X, -InnerRing2.Y, InnerRing2.Z)); //Back Left Top
	CubeVertices[15] = (FVector( InnerRing2.X, -InnerRing2.Y, InnerRing2.Z));//Front Left Top



	// with the vertices set it is time to give it a skin.
	// there is a pattern here that I messed up at some point. my UVs are off
	// Front (X): 0-1-2-3
	TangentVertices = FProcMeshTangent(0.f, 1.f, 0.f);
	AddQuadMesh(CubeVertices[0], CubeVertices[1], CubeVertices[2], CubeVertices[3], triangleIndexer, TangentVertices); 

	//Left (-Y): 2-3-4-5
	TangentVertices = FProcMeshTangent(1.f, 0.f, 0.f);
	AddQuadMesh(CubeVertices[2], CubeVertices[3], CubeVertices[4], CubeVertices[5], triangleIndexer, TangentVertices); 

	//Back (-X): 4-5-6-7
	TangentVertices = FProcMeshTangent(0.f, -1.f, 0.f);
	AddQuadMesh(CubeVertices[4], CubeVertices[5], CubeVertices[6], CubeVertices[7], triangleIndexer, TangentVertices); 

	//Right (Y): 6-7-0-1
	TangentVertices = FProcMeshTangent(-1.f, 0.f, 0.f);
	AddQuadMesh(CubeVertices[6], CubeVertices[7], CubeVertices[0], CubeVertices[1], triangleIndexer, TangentVertices);

	//Bottom (-Z): 1-7-3-5
	TangentVertices = FProcMeshTangent(0.f, -1.f, 0.f);
	AddQuadMesh(CubeVertices[1], CubeVertices[7], CubeVertices[3], CubeVertices[5], triangleIndexer, TangentVertices); 

	//Top outer ring(Z): 6-0-4-2
	TangentVertices = FProcMeshTangent(0.f, 1.f, 0.f);
	AddQuadMesh(CubeVertices[6], CubeVertices[0], CubeVertices[8], CubeVertices[9], triangleIndexer, TangentVertices);
	AddQuadMesh(CubeVertices[6], CubeVertices[8], CubeVertices[4], CubeVertices[10], triangleIndexer, TangentVertices);
	AddQuadMesh(CubeVertices[9], CubeVertices[0], CubeVertices[11], CubeVertices[2], triangleIndexer, TangentVertices);
	AddQuadMesh(CubeVertices[10], CubeVertices[11], CubeVertices[4], CubeVertices[2], triangleIndexer, TangentVertices);

	//Top Inner Ring 1(Z): 6-0-4-2
	TangentVertices = FProcMeshTangent(0.f, 1.f, 0.f);
	AddQuadMesh(CubeVertices[8], CubeVertices[9], CubeVertices[12], CubeVertices[13], triangleIndexer, TangentVertices);
	AddQuadMesh(CubeVertices[8], CubeVertices[12], CubeVertices[10], CubeVertices[14], triangleIndexer, TangentVertices);
	AddQuadMesh(CubeVertices[13], CubeVertices[9], CubeVertices[15], CubeVertices[11], triangleIndexer, TangentVertices);
	AddQuadMesh(CubeVertices[14], CubeVertices[15], CubeVertices[10], CubeVertices[11], triangleIndexer, TangentVertices);

	//Top Inner Quad 1(Z): 6-0-4-2
	TangentVertices = FProcMeshTangent(0.f, 1.f, 0.f);
	AddQuadMesh(CubeVertices[12], CubeVertices[13], CubeVertices[14], CubeVertices[15], triangleIndexer, TangentVertices);

	// with everything defined it is time to release it out in the world
	ThisMesh->ClearAllMeshSections();
	ThisMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
	ThisMesh->SetMaterial(0, Material);
}

void APM_Paramid::AddQuadMesh(FVector TopLeft, FVector BottomLeft, FVector TopRight, FVector BottomRight, int32& TriIndex)
{
	//GEngine->AddOnScreenDebugMessage(-1, 300.f, FColor::Red, FString::Printf(TEXT("Normal should be 1,0,0: %f, %f, %f"), sampledata.X, sampledata.Y,sampledata.Z ));

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

void APM_Paramid::AddQuadMesh(FVector TopLeft, FVector BottomLeft, FVector TopRight, FVector BottomRight, int32& TriIndex, FProcMeshTangent Tangent)
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
		Colors.Add(FLinearColor::Green);
	}
	UVs.Add(FVector2D(0.0f, 0.0f)); //top left
	UVs.Add(FVector2D(0.0f, 1.0f)); //bottom left
	UVs.Add(FVector2D(1.0f, 0.0f)); //top right
	UVs.Add(FVector2D(1.0f, 1.0f)); //bottom right
}

void APM_Paramid::AddTriangleMesh(FVector Point1, FVector Point2, FVector Point3, int32 & TriIndex, FProcMeshTangent Tangent)
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
		Colors.Add(FLinearColor::Green);
	}
	UVs.Add(FVector2D(0.0f, 0.0f)); //top left
	UVs.Add(FVector2D(0.0f, 1.0f)); //bottom left
	UVs.Add(FVector2D(1.0f, 0.0f)); //top right
}



void APM_Paramid::AddTriangle(int32 p1, int32 p2, int32 p3)
{
	// This is an over load method to the first one. I should move them together for reading flow
	Triangles.Add(p1);
	Triangles.Add(p2);
	Triangles.Add(p3);
}

void APM_Paramid::AddTriangle(int32 A, int32 B, int32 C, int32 D)
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

void APM_Paramid::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName MemberPropertyChanged = (PropertyChangedEvent.MemberProperty ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None);

	if ((MemberPropertyChanged == GET_MEMBER_NAME_CHECKED(APM_Paramid, PillarRise)))
	{
		GenerateMesh();
	}
	else if ((MemberPropertyChanged == GET_MEMBER_NAME_CHECKED(APM_Paramid, Material)))
	{
		ThisMesh->SetMaterial(0, Material);
	}
}
#endif // WITH_EDITOR