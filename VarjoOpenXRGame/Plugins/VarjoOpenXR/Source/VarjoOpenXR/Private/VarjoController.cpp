// Copyright 2023 Varjo Technologies Oy. All rights reserved.

#include "VarjoController.h"
#include "InputCoreTypes.h"
#include "OpenXRCore.h"
#include "OpenXRAssetDirectory.h"

#define LOCTEXT_NAMESPACE "VarjoController"

namespace VarjoOpenXR
{

    // Left
    const FKey VarjoController_Left_A_Click("VarjoController_Left_A_Click");
    const FKey VarjoController_Left_B_Click("VarjoController_Left_B_Click");
    const FKey VarjoController_Left_A_Touch("VarjoController_Left_A_Touch");
    const FKey VarjoController_Left_B_Touch("VarjoController_Left_B_Touch");
    const FKey VarjoController_Left_System_Click("VarjoController_Left_System_Click");
    const FKey VarjoController_Left_Grip_Click("VarjoController_Left_Grip_Click");
    const FKey VarjoController_Left_Grip_Touch("VarjoController_Left_Grip_Touch");
    const FKey VarjoController_Left_Trigger_Touch("VarjoController_Left_Trigger_Touch");
    const FKey VarjoController_Left_Trigger_Axis("VarjoController_Left_Trigger_Axis");
    const FKey VarjoController_Left_Thumbstick_X("VarjoController_Left_Thumbstick_X");
    const FKey VarjoController_Left_Thumbstick_Y("VarjoController_Left_Thumbstick_Y");
    const FKey VarjoController_Left_Thumbstick_2D("VarjoController_Left_Thumbstick_2D");
    const FKey VarjoController_Left_Thumbstick_Click("VarjoController_Left_Thumbstick_Click");
    const FKey VarjoController_Left_Thumbstick_Touch("VarjoController_Left_Thumbstick_Touch");

    //Right
    const FKey VarjoController_Right_A_Click("VarjoController_Right_A_Click");
    const FKey VarjoController_Right_B_Click("VarjoController_Right_B_Click");
    const FKey VarjoController_Right_A_Touch("VarjoController_Right_A_Touch");
    const FKey VarjoController_Right_B_Touch("VarjoController_Right_B_Touch");
    const FKey VarjoController_Right_System_Click("VarjoController_Right_System_Click");
    const FKey VarjoController_Right_Grip_Click("VarjoController_Right_Grip_Click");
    const FKey VarjoController_Right_Grip_Touch("VarjoController_Right_Grip_Touch");
    const FKey VarjoController_Right_Trigger_Touch("VarjoController_Right_Trigger_Touch");
    const FKey VarjoController_Right_Trigger_Axis("VarjoController_Right_Trigger_Axis");
    const FKey VarjoController_Right_Thumbstick_X("VarjoController_Right_Thumbstick_X");
    const FKey VarjoController_Right_Thumbstick_Y("VarjoController_Right_Thumbstick_Y");
    const FKey VarjoController_Right_Thumbstick_2D("VarjoController_Right_Thumbstick_2D");
    const FKey VarjoController_Right_Thumbstick_Click("VarjoController_Right_Thumbstick_Click");
    const FKey VarjoController_Right_Thumbstick_Touch("VarjoController_Right_Thumbstick_Touch");

    FSoftObjectPath VarjoLeftControllerPath = FString(TEXT("/VarjoOpenXR/Devices/Models/Varjo/varjo_controller_1_0_left.varjo_controller_1_0_left"));
    FSoftObjectPath VarjoRightControllerPath = FString(TEXT("/VarjoOpenXR/Devices/Models/Varjo/varjo_controller_1_0_right.varjo_controller_1_0_right"));

    void FVarjoController::Register()
    {
        RegisterOpenXRExtensionModularFeature();

        EKeys::AddMenuCategoryDisplayInfo("VarjoController",
            LOCTEXT("VarjoControllerSubCategory", "Varjo Controller"), TEXT("GraphEditor.PadEvent_16x"));

        // Left
        EKeys::AddKey(FKeyDetails(VarjoController_Left_A_Click,
            LOCTEXT("VarjoController_Left_A_Click", "Varjo Controller (L) A Press"),
            FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Left_B_Click,
            LOCTEXT("VarjoController_Left_B_Click", "Varjo Controller (L) B Press"),
            FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Left_A_Touch,
            LOCTEXT("VarjoController_Left_A_Touch", "Varjo Controller (L) A Touch"),
            FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Left_B_Touch,
            LOCTEXT("VarjoController_Left_B_Touch", "Varjo Controller (L) B Touch"),
            FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Left_System_Click,
            LOCTEXT("VarjoController_Left_System_Click", "Varjo Controller (L) Menu Press"),
            FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Left_Grip_Click,
            LOCTEXT("VarjoController_Left_Grip_Click", "Varjo Controller (L) Grip Click"),
            FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Left_Grip_Touch,
            LOCTEXT("VarjoController_Left_Grip_Touch", "Varjo Controller (L) Grip Touch"),
            FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Left_Trigger_Touch,
            LOCTEXT("VarjoController_Left_Trigger_Touch", "Varjo Controller (L) Trigger Touch"),
            FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Left_Trigger_Axis,
            LOCTEXT("VarjoController_Left_Trigger_Axis", "Varjo Controller (L) Trigger Axis"),
            FKeyDetails::GamepadKey | FKeyDetails::Axis1D | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Left_Thumbstick_X,
            LOCTEXT("VarjoController_Left_Thumbstick_X", "Varjo Controller (L) Thumbstick X-Axis"),
            FKeyDetails::GamepadKey | FKeyDetails::Axis1D | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Left_Thumbstick_Y,
            LOCTEXT("VarjoController_Left_Thumbstick_Y", "Varjo Controller (L) Thumbstick Y-Axis"),
            FKeyDetails::GamepadKey | FKeyDetails::Axis1D | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddPairedKey(FKeyDetails(VarjoController_Left_Thumbstick_2D,
            LOCTEXT("VarjoController_Left_Thumbstick_2D", "Varjo Controller (L) Thumbstick 2D-Axis"),
            FKeyDetails::GamepadKey | FKeyDetails::Axis2D | FKeyDetails::NotBlueprintBindableKey, "VarjoController"),
            VarjoController_Left_Thumbstick_X, VarjoController_Left_Thumbstick_Y);
        EKeys::AddKey(FKeyDetails(VarjoController_Left_Thumbstick_Click,
            LOCTEXT("VarjoController_Left_Thumbstick_Click", "Varjo Controller (L) Thumbstick Click"),
            FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Left_Thumbstick_Touch,
            LOCTEXT("VarjoController_Left_Thumbstick_Touch", "Varjo Controller (L) Thumbstick Touch"),
            FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));

        // Right
        EKeys::AddKey(FKeyDetails(VarjoController_Right_A_Click,
            LOCTEXT("VarjoController_Right_A_Click", "Varjo Controller (R) A Press"),
            FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Right_B_Click,
            LOCTEXT("VarjoController_Right_B_Click", "Varjo Controller (R) B Press"),
            FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Right_A_Touch,
            LOCTEXT("VarjoController_Right_A_Touch", "Varjo Controller (R) A Touch"),
            FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Right_B_Touch,
            LOCTEXT("VarjoController_Right_B_Touch", "Varjo Controller (R) B Touch"),
            FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Right_System_Click,
            LOCTEXT("VarjoController_Right_System_Click", "Varjo Controller (R) Menu"),
            FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Right_Grip_Click,
            LOCTEXT("VarjoController_Right_Grip_Click", "Varjo Controller (R) Grip Click"),
            FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Right_Grip_Touch,
            LOCTEXT("VarjoController_Right_Grip_Touch", "Varjo Controller (R) Grip Touch"),
            FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Right_Trigger_Touch,
            LOCTEXT("VarjoController_Right_Trigger_Touch", "Varjo Controller (R) Trigger Touch"),
            FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Right_Trigger_Axis,
            LOCTEXT("VarjoController_Right_Trigger_Axis", "Varjo Controller (R) Trigger Axis"),
            FKeyDetails::GamepadKey | FKeyDetails::Axis1D | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Right_Thumbstick_X,
            LOCTEXT("VarjoController_Right_Thumbstick_X", "Varjo Controller (R) Thumbstick X-Axis"),
            FKeyDetails::GamepadKey | FKeyDetails::Axis1D | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Right_Thumbstick_Y,
            LOCTEXT("VarjoController_Right_Thumbstick_Y", "Varjo Controller (R) Thumbstick Y-Axis"),
            FKeyDetails::GamepadKey | FKeyDetails::Axis1D | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddPairedKey(FKeyDetails(VarjoController_Right_Thumbstick_2D,
            LOCTEXT("VarjoController_Right_Thumbstick_2D", "Varjo Controller (R) Thumbstick 2D-Axis"),
            FKeyDetails::GamepadKey | FKeyDetails::Axis2D | FKeyDetails::NotBlueprintBindableKey, "VarjoController"),
            VarjoController_Right_Thumbstick_X, VarjoController_Right_Thumbstick_Y);
        EKeys::AddKey(FKeyDetails(VarjoController_Right_Thumbstick_Click,
            LOCTEXT("VarjoController_Right_Thumbstick_Click", "Varjo Controller (R) Thumbstick Click"),
            FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
        EKeys::AddKey(FKeyDetails(VarjoController_Right_Thumbstick_Touch,
            LOCTEXT("VarjoController_Right_Thumbstick_Touch", "Varjo Controller (R) Thumbstick Touch"),
            FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "VarjoController"));
    }

    void FVarjoController::Unregister()
    {
        IModularFeatures::Get().UnregisterModularFeature(GetModularFeatureName(), this);
    }

    bool FVarjoController::GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions){
        // TODO: use XR_VARJO_XR4_CONTROLLER_INTERACTION_EXTENSION_NAME  when openxr header has been updated
        OutExtensions.Add("XR_VARJO_xr4_controller_interaction");
        return true;
    }

    void FVarjoController::PostCreateInstance(XrInstance InInstance)
    {
        XrResult Result = xrStringToPath(InInstance, "/interaction_profiles/varjo/xr-4_controller", &InteractionProfile);
        check(XR_SUCCEEDED(Result));

        XrPath Path;
        Result = xrStringToPath(InInstance, "/user/hand/left", &Path);
        check(XR_SUCCEEDED(Result));
        ControllerModels.Add(Path, VarjoLeftControllerPath);

        Result = xrStringToPath(InInstance, "/user/hand/right", &Path);
        check(XR_SUCCEEDED(Result));
        ControllerModels.Add(Path, VarjoRightControllerPath);
    }

    bool FVarjoController::GetInteractionProfile(XrInstance InInstance, FString& OutKeyPrefix, XrPath& OutPath, bool& OutHasHaptics)
    {
        OutKeyPrefix = "VarjoController";
        OutPath = InteractionProfile;
        OutHasHaptics = true;

        return true;
    }

    bool FVarjoController::GetControllerModel(XrInstance InInstance, XrPath InInteractionProfile, XrPath InDevicePath, FSoftObjectPath& OutPath)
    {
        if (InInteractionProfile == InteractionProfile)
        {
            OutPath = ControllerModels[InDevicePath];
            return true;
        }

        return false;
    }

    void FVarjoController::GetControllerModelsForCooking(TArray<FSoftObjectPath>& OutPaths)
    {
        OutPaths.Add(VarjoLeftControllerPath);
        OutPaths.Add(VarjoRightControllerPath);
    }

}

#undef LOCTEXT_NAMESPACE
