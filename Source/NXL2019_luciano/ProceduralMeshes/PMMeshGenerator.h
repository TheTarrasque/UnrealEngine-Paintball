/*
Author: Devan Luciano
Inspiration Sources:
	Author Unknown: https://wiki.unrealengine.com/index.php?title=Procedural_Mesh_Component_in_C%2B%2B:Getting_Started
	SiggiG: https://github.com/SiggiG/ProceduralMeshes
	Koderz: https://github.com/Koderz/RuntimeMeshComponent
Class Purpose:
Building block code for all other ProceduralMeshComponents

 Primary methods/properties:

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
#include "PMMeshGenerator.generated.h"

UCLASS()
class NXL2019_LUCIANO_API APMMeshGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APMMeshGenerator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh parameters", meta = (ClampMin = "0.1", ClampMax= "1000.0", UIClampMin = "0.1", UIClampMax = "1000.0"))
	FVector QuadRadius = FVector(100.f, 100.f, 100.f);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif	//WITH_EDITOR
protected:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* ThisScene;
	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent* ThisMesh;

	virtual void PostActorCreated() override;
	virtual void PostLoad() override;

	void GenerateMesh();

private:
	TArray<FVector>Vertices;
	TArray<int32>Triangles;
	TArray<FVector>Normals;
	TArray<FProcMeshTangent>Tangents;
	TArray<FVector2D>UVs;
	TArray<FLinearColor>Colors;

	void AddTriangleMesh(FVector TopRight, FVector BottomRight, FVector BottomLeft, int32& TriIndex, FProcMeshTangent Tangent);
	void AddQuadMesh(FVector TopRight, FVector BottomRight, FVector TopLeft, FVector BottomLeft, int32& TriIndex, FProcMeshTangent Tangent);



};
