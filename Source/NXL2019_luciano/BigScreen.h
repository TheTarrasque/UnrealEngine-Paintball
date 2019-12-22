/*
Author: Devan Luciano
Inspiration Sources:
	UnrealEngine Youtube:https://www.youtube.com/watch?v=R0Y8EHEzBoY&index=2&list=LLmdii6_7MYxwlPE4cLXmZKg&t=2536s 
Class Purpose:
This class will pull in web resources and will be use to display each teams logos in a jumbo-tron manner
The primary function of this class is to setup a plane mesh to a Render target material instance. The tricky part is taking the Blueprint
script and converting to C++. 
Notes:
I tried creating this as a plugin class but UE4 put the class files in my source folder instead of the plugin folder. Must have messed up somewhere an I will try to fix
it later.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BigScreen.generated.h"

UCLASS()
class NXL2019_LUCIANO_API ABigScreen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABigScreen();

	UPROPERTY(EditAnywhere, Category = "Screen Properties")
	USceneComponent* ThisScene;
	UPROPERTY(EditAnywhere, Category = "Screen Properties")
	UStaticMeshComponent* ThisMesh;
	
	//will be use as a way for editor developers to copy and paste a web URL in 
	UPROPERTY(EditAnywhere, Category = "Screen Properties")
	FString ImageURL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Parameters")
	UMaterialInterface* Material;

	UMaterialInstanceDynamic* DynamicMaterial;


protected:
	virtual void BeginPlay() override;
public:	

	
	
};
