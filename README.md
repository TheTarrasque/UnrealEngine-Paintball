# UE4-Paintball
This project is designed to demonstrate how to use Unreal Engine's built in ProceduralMeshComponent and Koderz's RuntimeMeshComponent to create in game objects using C++. Each element not only has a define shape but also provide Unreal editors the opportunity to adjust object properties with in the editor. I would be remised if I left out a huge thanks to SiggiG for his work has really be instrumental in me gaining the knowledge needed to better understand the concepts presented here.

### Prerequisites

This project was designed with Unreal Engine 4.21.2 and Visual Studio 2017. As part of the project setup be sure to include the ProceduralMeshComponent into your public dependencies found in your ProjectName.build.cs file like so.
```
	public PM_Objects(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            **"ProceduralMeshComponent"**
        });

```
## Authors

**Devan Luciano** 

##Inspiration Sources:
Author Unknown: https://wiki.unrealengine.com/index.php?title=Procedural_Mesh_Component_in_C%2B%2B:Getting_Started
SiggiG: https://github.com/SiggiG/ProceduralMeshes
Koderz: https://github.com/Koderz/RuntimeMeshComponent
## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
