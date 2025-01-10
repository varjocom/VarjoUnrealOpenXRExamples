# Varjo OpenXR Plugin Changelog

All notable changes to this plugin are documented in this file from version 1.2.3 onwards.

## 1.2.5

No changes. New plugin version for UE 5.5 release.

## 1.2.4

No changes. New plugin version for UE 5.4 release.

## 1.2.3

Compatible with Varjo Base 4.0 or newer.

### Added

- Support for Varjo Controllers
- Support for XR-4
- Support for Unreal Engine 5.4
    - For engine versions 5.4.0 and 5.4.1 the plugin sets 'xr.RetainPreInitInstance' to True as
      a workaround to a crash. The crash is fixed for 5.4.2 by this commit
      https://github.com/EpicGames/UnrealEngine/commit/265cf72abb1f40855c50a64d53b7a13272c97180


### Fixed

- Fixed an issue where Quad View rendering mode was used in packaged executable even though Stereo rendering Mode was selected
- Fixed Depth Estimation functions not working properly when called during BeginPlay()


## Begin of changelog

No documented changes available from previous versions.
