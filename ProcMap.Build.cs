// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProcMap : ModuleRules
{
	public ProcMap(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "ProceduralMeshComponent" });
		PublicDependencyModuleNames.Add("RuntimeMeshComponent");
	}
}
