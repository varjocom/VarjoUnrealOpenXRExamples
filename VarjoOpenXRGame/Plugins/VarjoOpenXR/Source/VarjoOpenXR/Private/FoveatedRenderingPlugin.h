// Copyright 2021 Varjo Technologies Oy. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "OpenXRCommon.h"

namespace VarjoOpenXR
{
    class FFoveatedRenderingPlugin : public IOpenXRExtensionPlugin
    {
    public:
        void Register();
        void Unregister();

        /** IOpenXRExtensionPlugin implementation */
        virtual bool GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions) override;
        virtual void PostGetSystem(XrInstance InInstance, XrSystemId InSystem) override;
        virtual void PostCreateSession(XrSession InSession) override;
        virtual void* OnEnumerateViewConfigurationViews(XrInstance InInstance, XrSystemId InSystem, XrViewConfigurationType InViewConfigurationType, uint32_t InViewIndex, void* InNext) override;
        virtual const void* OnLocateViews(XrSession InSession, XrTime InDisplayTime, const void* InNext) override;

        /** Plugin functions */
        bool IsFoveatedRenderingSupported();
        bool IsFoveatedRenderingEnabled();

    private:
        bool bFoveatedRenderingSupported = false;
        XrSpace ViewSpace = XR_NULL_HANDLE;
        XrSpace RenderGazeSpace = XR_NULL_HANDLE;
        bool bFoveatedRenderingEnabledThisFrame = false;
        XrFoveatedViewConfigurationViewVARJO FoveatedViewConfigurationView[4];
        XrViewLocateFoveatedRenderingVARJO ViewLocateFoveatedRendering;
    };
}
