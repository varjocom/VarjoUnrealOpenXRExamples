// Copyright 2022 Varjo Technologies Oy. All rights reserved.

#pragma once

#include "OpenXRCommon.h"
#include "CoreMinimal.h"

namespace VarjoOpenXR
{
    class FMixedRealityPlugin : public IOpenXRExtensionPlugin
    {
    public:
        void Register();
        void Unregister();

        /** IOpenXRExtensionPlugin implementation */
        virtual bool GetOptionalExtensions(TArray<const ANSICHAR*>& OutExtensions) override;
        virtual void PostCreateInstance(XrInstance InInstance) override;
        virtual const void* OnBeginSession(XrSession InSession, const void* InNext) override;
        virtual void* OnEnumerateViewConfigurationViews(XrInstance InInstance, XrSystemId InSystem, XrViewConfigurationType InViewConfigurationType, uint32_t InViewIndex, void* InNext) override;

        bool IsMixedRealitySupported();
        bool IsMixedRealityEnabled();

        bool SetViewOffset(float Offset);
        void GetViewOffset(float& Offset);

    private:
        XrSession Session = XR_NULL_HANDLE;

        bool bBlendModesEnumerated = false;
        bool bMixedRealitySupported = false;
        XrEnvironmentBlendMode PrimaryBlendMode;

        PFN_xrSetViewOffsetVARJO xrSetViewOffsetVARJO = nullptr;
        bool bViewOffsetSet = false;
        float ViewOffset = 0.0f;
    };
}