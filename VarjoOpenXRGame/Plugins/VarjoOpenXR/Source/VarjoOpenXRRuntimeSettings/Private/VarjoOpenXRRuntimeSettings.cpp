// Copyright 2021 Varjo Technologies Oy. All rights reserved.

#include "VarjoOpenXRRuntimeSettings.h"

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

#include "HAL/IConsoleManager.h"

DEFINE_LOG_CATEGORY(LogVarjoOpenXRSettings);

IMPLEMENT_MODULE(FDefaultModuleImpl, VarjoOpenXRRuntimeSettings);


void UVarjoOpenXRRuntimeSettings::PostInitProperties()
{
    Super::PostInitProperties();

    // Setup callback for changing internal property when the CVar is changed

    auto OnCVarChanged = [this](IConsoleVariable* Variable)
    {
        int32 CVarValue = Variable->GetInt();
        if (CVarValue == 2)
        {
            RenderingMode = VarjoRenderingMode_Stereo;
        }
        else
        {
            RenderingMode = VarjoRenderingMode_QuadView;
        }
    };

    CVarReferenceOpenXRPreferredViewConfiguration = IConsoleManager::Get().FindConsoleVariable(TEXT("xr.OpenXRPreferredViewConfiguration"));
    if (CVarReferenceOpenXRPreferredViewConfiguration)
    {
        CVarReferenceOpenXRPreferredViewConfiguration->AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateLambda(OnCVarChanged));

        // Set CVar based on the saved setting, as the console variable is reset to default on editor restart
        if (RenderingMode == VarjoRenderingMode_Stereo)
        {
            CVarReferenceOpenXRPreferredViewConfiguration->Set(TEXT("2"), ECVF_SetByProjectSetting);
        }
        else
        {
            // 0 is the default value. Default XrViewConfigurationType of the XR runtime will be used
            CVarReferenceOpenXRPreferredViewConfiguration->Set(TEXT("0"), ECVF_SetByProjectSetting);
        }
    }
}

#if WITH_EDITOR
bool UVarjoOpenXRRuntimeSettings::CanEditChange(const FProperty* InProperty) const
{
    const bool ParentVal = Super::CanEditChange(InProperty);

    if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UVarjoOpenXRRuntimeSettings, FoveatedRendering))
    {
        return ParentVal && RenderingMode == VarjoRenderingMode_QuadView;
    }

    return ParentVal;
}

void UVarjoOpenXRRuntimeSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // Update the CVar to match the current state of settings
    if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UVarjoOpenXRRuntimeSettings, RenderingMode))
    {
        if (!CVarReferenceOpenXRPreferredViewConfiguration)
            return;

        if (RenderingMode == VarjoRenderingMode_Stereo)
        {
            CVarReferenceOpenXRPreferredViewConfiguration->Set(TEXT("2"), ECVF_SetByProjectSetting);
        }
        else
        {
            // 0 is the default value. Default XrViewConfigurationType of the XR runtime will be used
            CVarReferenceOpenXRPreferredViewConfiguration->Set(TEXT("0"), ECVF_SetByProjectSetting);
        }
    }
}
#endif // WITH_EDITOR