/*
Author: Devan Luciano
Inspiration Sources:
	Author Unknown: https://wiki.unrealengine.com/index.php?title=Procedural_Mesh_Component_in_C%2B%2B:Getting_Started
	SiggiG: https://github.com/SiggiG/ProceduralMeshes
	Koderz: https://github.com/Koderz/RuntimeMeshComponent
Class Purpose:
The JerseyBarrier will be used to seperate the game play area. Providing obsticles the players have to go around and will provide
the players with cover from one another

 Primary methods/properties:
	JerseyBarrierHeight: A property that when set in the editor will trigger the generate method and will determine the wall's height
	AddTriangleMesh: Will take three vectors and a Tangent (STILL NEED TO KNOW THE FORMULA For tangents!!) and will apply a mesh accordingly.
	AddQuadMesh: Will take Four vectors and a Tangent (STILL NEED TO KNOW THE FORMULA For tangents!!) and will apply a mesh accordingly.
	PostEditChangeProperty: Method that is triggered when a property of this actor is changed in the editor. If it is a key property then a method will be triggered

Additional Notes: I made an active effort to learn from the inspirational sources and not to copy their code. I wanted to make sure I truly understood the process
and am able to leave being able to take on the next bigger challenges. I also noticed that for the most part in these basic examples they hard coded all of it and did
not use methods like I decided to. I am sure it is because for basic shapes, it is actually less lines of code to just hard code it, even with the redundant lines.
However, I knew I wanted something scalable. Something that would allow me to create complete shapes that might have a real dynamic aspect to them.

02/03/2019: I made the decision to make these components the walls and obsticle in the game. They will still changed colors dynamically at run time, but the height 
controls are more a level designer option now version an ingame gimic

Final Project Rubric elements:
	Objects and Actors: 
		Key Properties - The Jerseybarrier height is a property that can be set from with the editor by the design team.
						also there are a few other properties that can be changed/set in the editor (MaterialA, MaterialB, and DynamicMaterialInstance)
		Visual Characteristics - The output of this class is an object that represents a typical jersey barrier. 
		Mathematical APIs - Adjusting the Jerseybarrier height property will dynamically change the actors height in the editor. Testing has
											has also showed this to work at run time as well. I depreticated that feature though.
		Lighting Functions- I am not quite sure what this is looking for, but my best guess says I did it.

	2D Texture and Shader properties:
		Basic Material- The base material is set in the editor as MaterialA, The MaterialB is designed to be the team's color and will indicate when 
						a player is taking cover up against a JB.
		Physical Characteristics - Not sure what this is looking for, hope I did it. 
		Shaders - This shader/material does not use math to alter color, but could've. I just didn't want to back track from what I had done. I believe I covered this in another actor.
		Shader Techniques or Macros - This actor uses a macro to trigger the runtime and editor property changes.

	Commented Code File:
		3D Object - I believe this is handled on line 138 of the source file.
		2D Texture and Material - This is done in the source file on lines 141, 230, and 239 
		Shader Technique - This actor is a basic barrier and therefore using a simple color material/shader and applying it to the object with no special specifications was the best method to use


*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RuntimeMeshComponent.h"
#include "RT_JerseyBarrier.generated.h"

UCLASS()
class NXL2019_LUCIANO_API ART_JerseyBarrier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ART_JerseyBarrier();

	virtual void PostInitializeComponents() override;

	// the default radius is set to 50 but can be adjusted in the editor as well
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Parameters")
	FVector ShapeRadius = FVector(100, 100.f, 100.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh parameters", meta = (ClampMin = "100.0", ClampMax = "1000.0", UIClampMin = "100.0", UIClampMax = "1000.0"))
	float JerseyBarrierHeight= 100.f;

	// Where I don t set a default material it can be set within the editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Parameters")
		UMaterialInterface* MaterialA;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Parameters")
		UMaterialInterface* MaterialB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Parameters")
	UMaterialInstanceDynamic* DynamicMaterialInstance;


#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif //WITH_EDITOR
protected:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* ThisScene;

	UPROPERTY(VisibleAnywhere)
		URuntimeMeshComponent* ThisMesh;


	UPROPERTY(VisibleAnywhere)
	UBoxComponent* ThisCollision;

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


	void AddQuadMesh(FVector TopLeft, FVector BottomLeft, FVector TopRight, FVector BottomRight, int32& TriIndex);//when you don t know the normals or tangents, wing it
	void AddQuadMesh(FVector TopLeft, FVector BottomLeft, FVector TopRight, FVector BottomRight, int32& TriIndex, FRuntimeMeshTangent Tangent);
	void AddTriangleMesh(FVector Point1, FVector Point2, FVector Point, int32& TriIndex, FRuntimeMeshTangent Tangent);

	void AddTriangle(int32 P1, int32 p2, int32 p3);
	void AddTriangle(int32 A, int32 B, int32 C, int32 D);


	virtual void NotifyActorBeginOverlap(AActor* OtherActor);
	virtual void NotifyActorEndOverlap(AActor* OtherActor);

};