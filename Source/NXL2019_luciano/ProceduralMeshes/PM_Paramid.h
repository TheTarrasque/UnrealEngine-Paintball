/*
Author: Devan Luciano
Inspiration Sources:
	Author Unknown: https://wiki.unrealengine.com/index.php?title=Procedural_Mesh_Component_in_C%2B%2B:Getting_Started
	SiggiG: https://github.com/SiggiG/ProceduralMeshes
	Koderz: https://github.com/Koderz/RuntimeMeshComponent
Class Purpose:
The Paramid is actually a player launcher. When the player goes to the top of the paramid it will trigger and event that will launch the player a 1000 units up. Birds 
Eye View, but very exposed. Would you risk it?

 Primary methods/properties:
	PillarRise: Property that can be set in the editor, but more importantly will be triggered from code on collision with the player
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
#include "PM_Paramid.generated.h"

UCLASS()
class NXL2019_LUCIANO_API APM_Paramid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APM_Paramid();

	// I decided not to mess with the radius on this object and so the defaults are as follows
		FVector ShapeBase = FVector(300, 300.f, 50.f);

		//used to create the base of the object to give the paramid the desired shape
		FVector ShapeHeight = ShapeBase * 0.45f;

		//Will allow the pillar's starting height to be edited in the editor. 
		//More importantly the pillar height will be changed on collision with the player while playing
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Parameters", meta = (ClampMin = "0.0", ClampMax = "1000.0", UIMin = "0", UIMax = "1000.0"))
		float PillarRise = 0.f;

		// Allows the material to be changed in the editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Parameters")
		UMaterialInterface* Material;


	// the trigger that senses changes in the editor
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif //WITH_EDITOR

protected:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* ThisScene;

	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent* ThisMesh;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* ThisTrigger;

	virtual void PostActorCreated() override;
	virtual void PostLoad() override;

	void GenerateMesh();



private:
	//Standard Mesh Variables
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;
	TArray<FVector2D> UVs;
	TArray<FLinearColor> Colors;

	//Methods used to generate the meshes
	void AddQuadMesh(FVector TopLeft, FVector BottomLeft, FVector TopRight, FVector BottomRight, int32& TriIndex);//when you don t know the normals or tangents, wing it
	void AddQuadMesh(FVector TopLeft, FVector BottomLeft, FVector TopRight, FVector BottomRight, int32& TriIndex, FProcMeshTangent Tangent);
	void AddTriangleMesh(FVector Point1, FVector Point2, FVector Point, int32& TriIndex, FProcMeshTangent Tangent);

	// in this Class I kept the Triangle processing seperate from the other components. This helped to understand the order
	void AddTriangle(int32 P1, int32 p2, int32 p3);
	void AddTriangle(int32 A, int32 B, int32 C, int32 D);

	virtual void NotifyActorBeginOverlap(AActor* OtherActor);
	virtual void NotifyActorEndOverlap(AActor* OtherActor);
};