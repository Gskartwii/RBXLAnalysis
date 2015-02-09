# RBXLAnalysis
An application for converting .rbxl files into a VCS-friendly form.

## Installing

### Cygwin
Clone the repo, get Cygwin with g++ and compile with the following command:

    g++ -O2 -Wall -finline-functions analysis.cpp -o analysis

### GNU/Linux
Clone the repo, get build-essentials and compile with the following command:

    g++ -O2 -Wall -finline-functions analysis.cpp -o analysis

## Usage
I plan on adding more options to the app later, but for now it can only be used to dump the information.
Use the following command to dump:

    analysis /path/to/rbxl-file

It will create a new directory called "extract" containing all the possible properties of the instances inside the file.

## Example
Example dump (`analysis Baseplate.rbxl && ls -aR extract`)

	AdService.AdService
	AssetService.AssetService
	BadgeService.BadgeService
	ChangeHistoryService.ChangeHistoryService
	CollectionService.CollectionService
	ContextActionService.ContextActionService
	CookiesService.CookiesService
	CSGDictionaryService.CSGDictionaryService
	Debris.Debris
	FilteredSelection.Instance
	FilteredSelection.Instance.1
	FilteredSelection.Instance.2
	GamePassService.GamePassService
	Geometry.Geometry
	HttpService.HttpService
	Instance.FWService
	Instance.LuaWebService
	Instance.ScriptInformationProvider
	Instance.ScriptService
	Instance.TimerService
	Instance.UserInputService
	Lighting.Lighting
	LogService.LogService
	NonReplicatedCSGDictionaryService.NonReplicatedCSGDictionaryService
	NotificationService.NotificationService
	PhysicsService.PhysicsService
	Players.Players
	PointsService.PointsService
	RenderHooksService.RenderHooksService
	ReplicatedFirst.ReplicatedFirst
	ReplicatedStorage.ReplicatedStorage
	Selection.Selection
	ServerScriptService.ServerScriptService
	ServerStorage.ServerStorage
	SocialService.SocialService
	Soundscape.SoundService
	StarterGui.StarterGui
	StarterPack.StarterPack
	StarterPlayer.StarterPlayer
	Teleport Service.TeleportService
	Workspace.Workspace

	extract/AdService.AdService:
	.  ..  properties.rbx_props

	extract/AssetService.AssetService:
	.  ..  properties.rbx_props

	extract/BadgeService.BadgeService:
	.  ..  properties.rbx_props

	extract/ChangeHistoryService.ChangeHistoryService:
	.  ..  properties.rbx_props

	extract/CollectionService.CollectionService:
	.  ..  properties.rbx_props

	extract/ContextActionService.ContextActionService:
	.  ..  properties.rbx_props

	extract/CookiesService.CookiesService:
	.  ..  properties.rbx_props

	extract/CSGDictionaryService.CSGDictionaryService:
	.  ..  properties.rbx_props

	extract/Debris.Debris:
	.  ..  properties.rbx_props

	extract/FilteredSelection.Instance:
	.  ..  properties.rbx_props

	extract/FilteredSelection.Instance.1:
	.  ..  properties.rbx_props

	extract/FilteredSelection.Instance.2:
	.  ..  properties.rbx_props

	extract/GamePassService.GamePassService:
	.  ..  properties.rbx_props

	extract/Geometry.Geometry:
	.  ..  properties.rbx_props

	extract/HttpService.HttpService:
	.  ..  properties.rbx_props

	extract/Instance.FWService:
	.  ..  properties.rbx_props

	extract/Instance.LuaWebService:
	.  ..  properties.rbx_props

	extract/Instance.ScriptInformationProvider:
	.  ..  properties.rbx_props

	extract/Instance.ScriptService:
	.  ..  properties.rbx_props

	extract/Instance.TimerService:
	.  ..  properties.rbx_props

	extract/Instance.UserInputService:
	.  ..  properties.rbx_props

	extract/Lighting.Lighting:
	.  ..  properties.rbx_props

	extract/LogService.LogService:
	.  ..  properties.rbx_props

	extract/NonReplicatedCSGDictionaryService.NonReplicatedCSGDictionaryService:
	.  ..  properties.rbx_props

	extract/NotificationService.NotificationService:
	.  ..  properties.rbx_props

	extract/PhysicsService.PhysicsService:
	.  ..  properties.rbx_props

	extract/Players.Players:
	.  ..  properties.rbx_props

	extract/PointsService.PointsService:
	.  ..  properties.rbx_props

	extract/RenderHooksService.RenderHooksService:
	.  ..  properties.rbx_props

	extract/ReplicatedFirst.ReplicatedFirst:
	.  ..  properties.rbx_props

	extract/ReplicatedStorage.ReplicatedStorage:
	.  ..  properties.rbx_props

	extract/Selection.Selection:
	.  ..  properties.rbx_props

	extract/ServerScriptService.ServerScriptService:
	.  ..  properties.rbx_props

	extract/ServerStorage.ServerStorage:
	.  ..  properties.rbx_props

	extract/SocialService.SocialService:
	.  ..  properties.rbx_props

	extract/Soundscape.SoundService:
	.  ..  properties.rbx_props

	extract/StarterGui.StarterGui:
	.  ..  properties.rbx_props

	extract/StarterPack.StarterPack:
	.  ..  properties.rbx_props

	extract/StarterPlayer.StarterPlayer:
	.  ..  properties.rbx_props

	extract/Teleport Service.TeleportService:
	.  ..  properties.rbx_props

	extract/Workspace.Workspace:
	.  ..  BasePlate.Part  Camera.Camera  properties.rbx_props  Terrain.Terrain

	extract/Workspace.Workspace/BasePlate.Part:
	.  ..  properties.rbx_props

	extract/Workspace.Workspace/Camera.Camera:
	.  ..  properties.rbx_props

	extract/Workspace.Workspace/Terrain.Terrain:
	.  ..  properties.rbx_props

What's inside `extract/Workspace.Workspace/BasePlate.Part/properties.rbx_props`:

    // RBXL Analysis Object Property Extraction
	// This file is not meant to be edited manually
	Anchored (Bool Entry): true
	BackParamA (Float Entry): 0.5 (0x3F000000)
	BackParamB (Float Entry): 0.5 (0x3F000000)
	BackSurface (Int32/Enum/Token Entry): 32596 (0x00007F54)
	BackSurfaceInput (Int32/Enum/Token Entry): 0 (0x00000000)
	BottomParamA (Float Entry): 0.5 (0x3F000000)
	BottomParamB (Float Entry): 0.5 (0x3F000000)
	BottomSurface (Int32/Enum/Token Entry): 32596 (0x00007F54)
	BottomSurfaceInput (Int32/Enum/Token Entry): 0 (0x00000000)
	BrickColor (BrickColor Entry): Dark stone gray (0x000000C7)
	CFrame (CFrame Entry): X: 0 (0x00000000), Y: -10 (0xC1200000), Z: 0 (0x00000000), Rotation Angles: {0, 0, 0}, Rotation Matrix: {1, 0, 0, 0, 1, 0, 0, 0, 1}, Rotation Angles Hex: {0x00000000, 0x00000000, 0x00000000}, Rotation Matrix Hex: {0x3F800000, 0x00000000, 0x00000000, 0x00000000, 0x3F800000, 0x00000000, 0x00000000, 0x00000000, 0x3F800000}
	CanCollide (Bool Entry): true
	Elasticity (Float Entry): 0.5 (0x3F000000)
	Friction (Float Entry): 0.3 (0x3E99999A)
	FrontParamA (Float Entry): 0.5 (0x3F000000)
	FrontParamB (Float Entry): 0.5 (0x3F000000)
	FrontSurface (Int32/Enum/Token Entry): 0 (0x00000000)
	FrontSurfaceInput (Int32/Enum/Token Entry): 32596 (0x00007F54)
	LeftParamA (Float Entry): 0.5 (0x3F000000)
	LeftParamB (Float Entry): 0.5 (0x3F000000)
	LeftSurface (Int32/Enum/Token Entry): 32596 (0x00007F54)
	LeftSurfaceInput (Int32/Enum/Token Entry): 32596 (0x00007F54)
	Locked (Bool Entry): true
	Material (Int32/Enum/Token Entry): 32596 (0x00007F54)
	Name (String Entry, 1 Lines): BasePlate
	Reflectance (Float Entry): 0 (0x00000000)
	RightParamA (Float Entry): 0.5 (0x3F000000)
	RightParamB (Float Entry): 0.5 (0x3F000000)
	RightSurface (Int32/Enum/Token Entry): 0 (0x00000000)
	RightSurfaceInput (Int32/Enum/Token Entry): 0 (0x00000000)
	RotVelocity (Vector3 Entry): X: 0(0x00000000), Y: 0(0x00000000), Z: 0(0x00000000)
	TopParamA (Float Entry): 0.5 (0x3F000000)
	TopParamB (Float Entry): 0.5 (0x3F000000)
	TopSurface (Int32/Enum/Token Entry): 32596 (0x00007F54)
	TopSurfaceInput (Int32/Enum/Token Entry): 32596 (0x00007F54)
	Transparency (Float Entry): 0 (0x00000000)
	Velocity (Vector3 Entry): X: 0(0x00000000), Y: 0(0x00000000), Z: 0(0x00000000)
	formFactorRaw (Int32/Enum/Token Entry): 32596 (0x00007F54)
	shape (Int32/Enum/Token Entry): 32596 (0x00007F54)
	size (Vector3 Entry): X: 512(0x44000000), Y: 20(0x41A00000), Z: 512(0x44000000)
