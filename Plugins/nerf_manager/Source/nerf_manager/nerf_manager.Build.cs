// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class nerf_manager : ModuleRules
{
	public nerf_manager(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core","InputCore" ,"UMG","ProceduralMeshComponent"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"RawMesh" ,
				"SlateCore","Renderer","RenderCore","RHI"
// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "..", "..", "lib", "ue4_plug_v3.lib"));
			RuntimeDependencies.Add(Path.Combine("$(PluginDir)/Binaries/Win64", "ue4_plug_v3.dll"), Path.Combine("$(PluginDir)/lib/ue4_plug_v3.dll"));


		}
		else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "..", "..", "lib", "libue4_plug_v3.so"));
			RuntimeDependencies.Add(Path.Combine("$(PluginDir)/Binaries/Linux", "libue4_plug_v3.so"), Path.Combine("$(PluginDir)/lib/libue4_plug_v3.so"));

		}
		else
		{

		}

	}
}
