// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Assignment2 : ModuleRules
{
	public Assignment2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Niagara" });
	}
}
