// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EldenRing : ModuleRules
{
	public EldenRing(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        OptimizeCode = CodeOptimization.Never;

        PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"AIModule",
            "NavigationSystem",
            "Chaos",
            "UMG",
            "GeometryCollectionEngine",
            "FieldSystemEngine",
            "HairStrandsCore",
            "GameplayCameras",
            "Niagara"
            //"Cascade"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Slate",
            "SlateCore"
        });
    }
}
