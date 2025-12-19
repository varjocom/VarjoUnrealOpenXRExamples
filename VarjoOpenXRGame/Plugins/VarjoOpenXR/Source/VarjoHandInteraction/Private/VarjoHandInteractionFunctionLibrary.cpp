// Copyright 2025 Varjo Technologies Oy. All rights reserved.

#include "VarjoHandInteractionFunctionLibrary.h"
#include "VarjoHandInteraction.h"

FVarjoHandInteraction* GetHandInteraction()
{
	TArray<FVarjoHandInteractionModule*> InputDeviceModules = IModularFeatures::Get().GetModularFeatureImplementations<FVarjoHandInteractionModule>(IInputDeviceModule::GetModularFeatureName());
	for (auto InputDeviceModule : InputDeviceModules)
	{
		if (InputDeviceModule->GetModuleKeyName().IsEqual(IVarjoHandInteractionModule::VarjoHandInteractionModularKeyName))
			return InputDeviceModule->GetHandInteraction();
	}

	return nullptr;
}

void UVarjoHandInteractionFunctionLibrary::GetHandInteractionAimPose(bool IsRight, bool& IsValid, FVector& Location, FRotator& Rotation)
{
	IsValid = false;
	Location = FVector::ZeroVector;
	Rotation = FRotator::ZeroRotator; 

	auto HandInteraction = GetHandInteraction();
	if (HandInteraction == nullptr) return;

	FName motionSource = HandInteractionMotionSource::LeftAim;
	if (IsRight) motionSource = HandInteractionMotionSource::RightAim;
	if (HandInteraction->GetControllerTrackingStatus(0, motionSource) == ETrackingStatus::Tracked)
	{
		IsValid = true;
		HandInteraction->GetControllerOrientationAndPosition(0, motionSource, Rotation, Location, HandInteraction->GetWorldToMeterScale());
	}
}

void UVarjoHandInteractionFunctionLibrary::GetHandInteractionGripPose(bool IsRight, bool& IsValid, FVector& Location, FRotator& Rotation)
{
	IsValid = false;
	Location = FVector::ZeroVector;
	Rotation = FRotator::ZeroRotator;

	auto HandInteraction = GetHandInteraction();
	if (HandInteraction == nullptr) return;

	FName motionSource = HandInteractionMotionSource::LeftGrip;
	if (IsRight) motionSource = HandInteractionMotionSource::RightGrip;
	if (HandInteraction->GetControllerTrackingStatus(0, motionSource) == ETrackingStatus::Tracked)
	{
		IsValid = true;
		HandInteraction->GetControllerOrientationAndPosition(0, motionSource, Rotation, Location, HandInteraction->GetWorldToMeterScale());
	}
}
