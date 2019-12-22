// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class NXL2019_lucianoEditorTarget : TargetRules
{
	public NXL2019_lucianoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("NXL2019_luciano");
	}
}
