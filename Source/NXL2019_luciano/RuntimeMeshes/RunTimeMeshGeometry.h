/*
Author: Devan Luciano
Inspiration Sources: 
	Author Unknown: https://wiki.unrealengine.com/index.php?title=Procedural_Mesh_Component_in_C%2B%2B:Getting_Started
	SiggiG: https://github.com/SiggiG/ProceduralMeshes
	Koderz: https://github.com/Koderz/RuntimeMeshComponent
Class Purpose:
 The RunTimeMeshGeomety class will serve as the template for all other RuntimeMeshComponent shapes that will be created in this project. 
 It will take the rundant code presented and make them more effecient and reduce the amount of code needed to create more complex shapes.
 Ideally by the end of the course, or shortly after, I will make this a class that others can be derived from rather than copied from.

 There are three methods that are key to the success of the class
	AddTriangleMesh: Will take three vectors and a Tangent (STILL NEED TO KNOW THE FORMULA For tangents!!) and will apply a mesh accordingly.
	AddQuadMesh: Will take Four vectors and a Tangent (STILL NEED TO KNOW THE FORMULA For tangents!!) and will apply a mesh accordingly.
	PostEditChangeProperty: Method that is triggered when a property of this actor is changed in the editor. If it is a key property then a method will be triggered

Additional Notes: I made an active effort to learn from the inspirational sources and not to copy their code. I wanted to make sure I truly understood the process
and am able to leave being able to take on the next bigger challenges. I also noticed that for the most part in these basic examples they hard coded all of it and did 
not use methods like I decided to. I am sure it is because for basic shapes, it is actually less lines of code to just hard code it, even with the redundant lines.
However, I knew I wanted something scalable. Something that would allow me to create complete shapes that might have a real dynamic aspect to them.

Final Project Rubric elements:
	Objects and Actors:
		Key Properties - The CubeRadius, the material, and the InnerEdgeSize properties are open and can be changed in the editor.

		Visual Characteristics - This will produce a 3D cube object into the world that has an adjustable inner portion. 
		This will be used as an obstacle in game and I may had a visual effect by adding an event tick to open and close the inner portions of the cubes

		Lighting Functions- I am not quite sure what this is looking for, but my best guess says I did it.

	2D Texture and Shader properties:
		Basic Material- The base material for this object is unique in that the grid pattern will changed depending on where in the game it is placed.
		Physical Characteristics - Not sure what this is looking for, hope I did it.
		Shaders - I used a dynamic shader for this object as mentioned before. There are several mathematical functions being used to accomplish the effect
		Shader Techniques or Macros - This actor uses a macro to trigger the runtime and editor property changes.

	Commented Code File:
		3D Object - I believe this is handled on line 254 of the source file.
		2D Texture and Material - This is done in the source file on lines 258-259
		Shader Technique - 

*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RuntimeMeshComponent.h"
#include "RunTimeMeshGeometry.generated.h"

UCLASS()
class NXL2019_LUCIANO_API ARunTimeMeshGeometry : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARunTimeMeshGeometry();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Parameters")
		FVector CubeRadius = FVector(50, 50.f, 50.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Parameters")
		UMaterialInterface* Material;

	// need to find out how to limit input between a certain range. this should be .0 to .9
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Parameters", meta = (ClampMin = "0.1", ClampMax = ".95", UIMin = ".1", UIMax = ".95"))
	float InnerEdgeSize = .9f;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif //WITH_EDITOR
protected:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* ThisScene;

	UPROPERTY(VisibleAnywhere)
		URuntimeMeshComponent* ThisMesh;

	virtual void PostActorCreated() override;
	virtual void PostLoad() override;

	void GenerateMesh();

private:
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FRuntimeMeshTangent> Tangents;
	TArray<FVector2D> UVs;
	TArray<FColor> Colors;

	void AddQuadMesh(FVector TopLeft, FVector BottomLeft, FVector TopRight, FVector BottomRight, int32& TriIndex);//when you dont know the normals or tangents, wing it
	void AddQuadMesh(FVector TopLeft, FVector BottomLeft, FVector TopRight, FVector BottomRight, int32& TriIndex, FRuntimeMeshTangent Tangent);
	void AddTriangleMesh(FVector Point1, FVector Point2, FVector Point, int32& TriIndex, FRuntimeMeshTangent Tangent);

	void AddTriangle(int32 P1, int32 p2, int32 p3);
	void AddTriangle(int32 A, int32 B, int32 C, int32 D);
};
