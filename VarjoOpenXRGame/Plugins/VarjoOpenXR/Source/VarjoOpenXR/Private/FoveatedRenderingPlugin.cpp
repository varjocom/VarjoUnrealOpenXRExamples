// Copyright 2021 Varjo Technologies Oy. All rights reserved.

#include "FoveatedRenderingPlugin.h"
#include "VarjoOpenXR.h"
#include "VarjoOpenXRRuntimeSettings.h"

namespace VarjoOpenXR
{
    void FFoveatedRenderingPlugin::Register()
    {
        RegisterOpenXRExtensionModularFeature();
    }

    void FFoveatedRenderingPlugin::Unregister()
    {
        UnregisterOpenXRExtensionModularFeature();
    }

    bool FFoveatedRenderingPlugin::GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions)
    {
        OutExtensions.Add(XR_VARJO_QUAD_VIEWS_EXTENSION_NAME);
        OutExtensions.Add(XR_VARJO_FOVEATED_RENDERING_EXTENSION_NAME);
        return true;
    }

    void FFoveatedRenderingPlugin::PostGetSystem(XrInstance InInstance, XrSystemId InSystem)
    {
        XrSystemFoveatedRenderingPropertiesVARJO FoveatedRenderingProperties{ XR_TYPE_SYSTEM_FOVEATED_RENDERING_PROPERTIES_VARJO, nullptr };
        XrSystemProperties SystemProperties{ XR_TYPE_SYSTEM_PROPERTIES, &FoveatedRenderingProperties };
        XR_ENSURE(xrGetSystemProperties(InInstance, InSystem, &SystemProperties));

        bFoveatedRenderingSupported = FoveatedRenderingProperties.supportsFoveatedRendering == XR_TRUE;
    }

    void FFoveatedRenderingPlugin::PostCreateSession(XrSession InSession)
    {
        if (!bFoveatedRenderingSupported)
        {
            return;
        }

        XrReferenceSpaceCreateInfo ViewSpaceCreateInfo{ XR_TYPE_REFERENCE_SPACE_CREATE_INFO, nullptr, XR_REFERENCE_SPACE_TYPE_VIEW };
        ViewSpaceCreateInfo.poseInReferenceSpace.orientation.w = 1.0f;
        XR_ENSURE(xrCreateReferenceSpace(InSession, &ViewSpaceCreateInfo, &ViewSpace));

        XrReferenceSpaceCreateInfo RenderGazeSpaceCreateInfo{ XR_TYPE_REFERENCE_SPACE_CREATE_INFO, nullptr, XR_REFERENCE_SPACE_TYPE_COMBINED_EYE_VARJO };
        RenderGazeSpaceCreateInfo.poseInReferenceSpace.orientation.w = 1.0f;
        XR_ENSURE(xrCreateReferenceSpace(InSession, &RenderGazeSpaceCreateInfo, &RenderGazeSpace));
    }

    // TODO It would be good to have a better fallback strategy for foveated rendering
    // Currently if the rendering gaze isn't available we're still rendering using foveated resolutions
    // And if we want to toggle foveated rendering dynamically, it requires swapchain reallocation
    // Unfortunately, we can't do it in a better way due to lack of Unreal Engine API
    void* FFoveatedRenderingPlugin::OnEnumerateViewConfigurationViews(XrInstance InInstance, XrSystemId InSystem, XrViewConfigurationType InViewConfigurationType, uint32_t InViewIndex, void* InNext)
    {
        // If we're using a different view configuration type return early, otherwise we may access FoveatedViewConfigurationView array out-of-bounds
        if (InViewConfigurationType != XR_VIEW_CONFIGURATION_TYPE_PRIMARY_QUAD_VARJO || !IsFoveatedRenderingEnabled())
        {
            bFoveatedRenderingEnabledThisFrame = false;
            return InNext;
        }

        bFoveatedRenderingEnabledThisFrame = true;

        check(InViewIndex < sizeof(FoveatedViewConfigurationView) / sizeof(FoveatedViewConfigurationView[0]));
        FoveatedViewConfigurationView[InViewIndex] = XrFoveatedViewConfigurationViewVARJO{ XR_TYPE_FOVEATED_VIEW_CONFIGURATION_VIEW_VARJO, InNext, bFoveatedRenderingEnabledThisFrame };
        return &FoveatedViewConfigurationView[InViewIndex];
    }

    const void* FFoveatedRenderingPlugin::OnLocateViews(XrSession InSession, XrTime InDisplayTime, const void* InNext)
    {
        if (!bFoveatedRenderingEnabledThisFrame || ViewSpace == XR_NULL_HANDLE || RenderGazeSpace == XR_NULL_HANDLE)
        {
            return InNext;
        }

        XrSpaceLocation RenderGazeLocation{ XR_TYPE_SPACE_LOCATION, nullptr };
        XR_ENSURE(xrLocateSpace(RenderGazeSpace, ViewSpace, InDisplayTime, &RenderGazeLocation));

        ViewLocateFoveatedRendering = XrViewLocateFoveatedRenderingVARJO{ XR_TYPE_VIEW_LOCATE_FOVEATED_RENDERING_VARJO, InNext, true };
        return &ViewLocateFoveatedRendering;
    }

    bool FFoveatedRenderingPlugin::IsFoveatedRenderingSupported()
    {
        return bFoveatedRenderingSupported;
    }

    bool FFoveatedRenderingPlugin::IsFoveatedRenderingEnabled()
    {
        const UVarjoOpenXRRuntimeSettings* Settings = GetDefault<UVarjoOpenXRRuntimeSettings>();
        check(Settings);
        return bFoveatedRenderingSupported && Settings->FoveatedRendering;
    }
}
