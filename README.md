# Varjo OpenXR
Varjo mixed and virtual reality headsets provide users with a highly immersive experience and leading visual quality. They enable you to leverage the full capabilities of Unreal Engine to develop highly visual and engaging immersive experiences.

Varjo OpenXR plugin provides support for Varjo OpenXR extensions, enabling you to develop best-in-class mixed reality applications. The plugin gives you access to the following features:
* Video pass-through mixed reality
* Depth occlusion
* Foveated rendering
* Varjo Markers

Built-in plugins OpenXREyeTracker and OpenXRHandTracking are enabled by default with the plugin, enabling eye tracking and hand tracking with Varjo devices.

## Branches
**master** - Master branch contains an example project for the latest released Unreal Engine and Varjo OpenXR plugin.
To be used with Varjo OpenXR plugin from Unreal Marketplace or developer.varjo.com.

**[version]** (e.g. 4.27) - Version branches contain an example project for the latest Varjo OpenXR plugin release for each supported minor engine version.
To be used with Varjo OpenXR plugin from Unreal Marketplace or developer.varjo.com.

**[version]-dev** (e.g. 4.27-dev) - Dev branches contain snapshots of an example project and Varjo OpenXR plugin still in development.
The plugin is included in the project and may include features which are not yet supported by the latest released Varjo OpenXR runtime.
It's not recommended to use the plugin from a development branch in production as it's not thoroughly tested and may not work properly.
Use development branches only if you want to experiment with unreleased engine versions or plugin features.

## Varjo OpenXR Examples
This repository contains an example project, showing you how to utilize the features of Varjo OpenXR plugin.

### Get Started
1. Clone this repository
2. Install Varjo OpenXR plugin to your engine from Unreal Marketplace or download the plugin from https://developer.varjo.com/ and place the unpacked plugin folder in `VarjoOpenXRGame\Plugins\`.
3. Open `VarjoOpenXRGame\VarjoOpenXRGame.uproject`.
