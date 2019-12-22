// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NXL2019_luciano : ModuleRules
{
	public NXL2019_luciano(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "HeadMountedDisplay",
            "ProceduralMeshComponent",
            "RuntimeMeshComponent",
            "RHI",
            "RenderCore",
            "PixelShader",
            "ComputeShader"

        });
	}
}
