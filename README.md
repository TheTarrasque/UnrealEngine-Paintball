# UE4-ProceduralMeshes
This project is designed to demonstrate how to use Unreal Engine's built in ProceduralMeshComponent to create in game objects using C++. Each element not only has a define shape but also provide Unreal editors the opportunity to adjust object properties with in the editor.

## Getting Started

If you are new to procedural programming the best place to start is by taking a look at the PM_CoreActor class. This is the foundation in which all the other objects are derived from.

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

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Authors

**Devan Luciano** 

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
