// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PracticeD : ModuleRules
{
	public PracticeD(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "PracticeD" });

		PublicDependencyModuleNames.AddRange(new string[] { "Core", 
			"CoreUObject",
			"Engine", 
			"InputCore", 
			"EnhancedInput" 
		
		});
	}
}
