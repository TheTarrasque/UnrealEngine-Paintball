/*
Author: Devan Luciano
Inspiration Sources:
	Author Unknown: https://wiki.unrealengine.com/index.php?title=Procedural_Mesh_Component_in_C%2B%2B:Getting_Started
	SiggiG: https://github.com/SiggiG/ProceduralMeshes
	Koderz: https://github.com/Koderz/RuntimeMeshComponent
Class Purpose:
The Original purpose was to use the ability to open and close as door. However, I realized that the inner cube actually fills in as well. 
This of course would crush the player. Therefor, I am repurposing this as my walls for my level. Time allowing I can see the ability of opening as being a side game 
for target shotting. Will also be used in the level to allow the player to take cover while still being able to shoot out at the other players.

 Primary methods/properties:
	InnerEdgeSize: Property used to dynamically change the inner space of the cube.
	AddTriangleMesh: Will take three vectors and a Tangent (STILL NEED TO KNOW THE FORMULA For tangents!!) and will apply a mesh accordingly.
	AddQuadMesh: Will take Four vectors and a Tangent (STILL NEED TO KNOW THE FORMULA For tangents!!) and will apply a mesh accordingly.
	PostEditChangeProperty: Method that is triggered when a property of this actor is changed in the editor. If it is a key property then a method will be triggered

Additional Notes: I made an active effort to learn from the inspirational sources and not to copy their code. I wanted to make sure I truly understood the process
and am able to leave being able to take on the next bigger challenges. I also noticed that for the most part in these basic examples they hard coded all of it and did
not use methods like I decided to. I am sure it is because for basic shapes, it is actually less lines of code to just hard code it, even with the redundant lines.
However, I knew I wanted something scalable. Something that would allow me to create complete shapes that might have a real dynamic aspect to them.


*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralGeometry.generated.h"

UCLASS()
class NXL2019_LUCIANO_API AProceduralGeometry : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralGeometry();

	// the default radius is set to 50 but can be adjusted in the editor as well
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Parameters")
	FVector CubeRadius = FVector(100, 100.f, 100.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Parameters")
	float InnerEdgeSize = .75f;

	// Where I dont set a default material it can be set within the editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Parameters")
	UMaterialInterface* Material;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif //WITH_EDITOR
protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* ThisScene;

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* ThisMesh;

	virtual void PostActorCreated() override;
	virtual void PostLoad() override;

	void GenerateMesh();

	

private:
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;
	TArray<FVector2D> UVs;
	TArray<FLinearColor> Colors;


	void AddQuadMesh(FVector TopLeft, FVector BottomLeft, FVector TopRight, FVector BottomRight, int32& TriIndex);//when you dont know the normals or tangents, wing it
	void AddQuadMesh(FVector TopLeft, FVector BottomLeft, FVector TopRight, FVector BottomRight, int32& TriIndex, FProcMeshTangent Tangent);
	void AddTriangleMesh(FVector Point1, FVector Point2, FVector Point,int32& TriIndex, FProcMeshTangent Tangent);

	void AddTriangle(int32 P1, int32 p2, int32 p3);
	void AddTriangle(int32 A, int32 B, int32 C, int32 D);



	
};
