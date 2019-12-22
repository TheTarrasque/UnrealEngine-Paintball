/*
Author: Devan Luciano
Inspiration Sources:
	Author Unknown: https://wiki.unrealengine.com/index.php?title=Procedural_Mesh_Component_in_C%2B%2B:Getting_Started
	SiggiG: https://github.com/SiggiG/ProceduralMeshes
	Koderz: https://github.com/Koderz/RuntimeMeshComponent
Class Purpose:
Standard cube from the first tutorials, modified of course. But with the modification I added the ability to set the mesh and radius from the editor. 
This was my first experiment with this ability and would be the launching pad for the next few shapes. Overall this shape will be just a filler obsticle in the game

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
		Key Properties - The CubeRadius the material properties is open and can be changed in the editor.

		Visual Characteristics - This will produce a 3D cube object into the world. Which will be used as an obsticle in game

		Lighting Functions- I am not quite sure what this is looking for, but my best guess says I did it.

	2D Texture and Shader properties:
		Basic Material- This object applies a 2D material that the designer can apply within the editor
		Physical Characteristics - Not sure what this is looking for, hope I did it.
		Shaders - I used a dynamic shader on a visual matrix rotating cube that is in the center of the level
		Shader Techniques or Macros - This actor uses a macro to trigger the runtime and editor property changes.

	Commented Code File:
		3D Object - I believe this is handled on line 79 of the source file.
		2D Texture and Material - This is done in the source file on lines 81
		Shader Technique - Standard material for the most part. However, I did use it as my featured mathematical shader as well. The texture is just a 
			typical matrix image. The shader/Material uses the screen position which is passed into a custom function. This function obtains the x position of the screen
			and reapplies the material onto the object at runtime


*/

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshComponent.h"
#include "GameFramework/Actor.h"
#include "RTCube.generated.h"

UCLASS()
class NXL2019_LUCIANO_API ARTCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARTCube();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh parameters", meta = (ClampMin = "0.1", ClampMax = "1000.0", UIClampMin = "0.1", UIClampMax = "1000.0"))
		FVector PMCubeRaduis = FVector(100.f, 100.f, 100.f);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif	//WITH_EDITOR


protected:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* ThisScene;
	UPROPERTY(VisibleAnywhere)
		URuntimeMeshComponent* ThisMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Parameters")
		UMaterialInterface* Material;

	virtual void PostActorCreated() override;
	virtual void PostLoad() override;

	void GenerateMesh();

private:
	TArray<FVector>Vertices;
	TArray<int32>Triangles;
	TArray<FVector>Normals;
	TArray<FRuntimeMeshTangent>Tangents;
	TArray<FVector2D>UVs;
	TArray<FColor>Colors;

	void AddTriangleMesh(FVector TopRight, FVector BottomRight, FVector BottomLeft, int32& TriIndex, FRuntimeMeshTangent Tangent);
	void AddQuadMesh(FVector TopRight, FVector BottomRight, FVector TopLeft, FVector BottomLeft, int32& TriIndex, FRuntimeMeshTangent Tangent);


};

