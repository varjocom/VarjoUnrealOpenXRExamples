// Copyright 2021 Varjo Technologies Oy. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "OpenXRCommon.h"
#include "VarjoOpenXR.h"

#include "Features/IModularFeatures.h"
#include "Features/IModularFeature.h"
#include "Delegates/Delegate.h"
#include "Delegates/DelegateCombinations.h"

namespace VarjoOpenXR
{

    class FVarjoMarkersPlugin : public IOpenXRExtensionPlugin
    {
    public:
        void Register();
        void Unregister();

        /** IOpenXRExtensionPlugin implementation */
        virtual bool GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions) override;
        virtual const void* OnCreateSession(XrInstance InInstance, XrSystemId InSystem, const void* InNext) override;
        virtual void PostGetSystem(XrInstance InInstance, XrSystemId InSystem) override;
        virtual void PostCreateSession(XrSession InSession) override;
        virtual void OnEvent(XrSession InSession, const XrEventDataBaseHeader* InHeader) override;
        virtual void UpdateDeviceLocations(XrSession InSession, XrTime DisplayTime, XrSpace TrackingSpace) override;

        /** Plugin functions */
        bool IsVarjoMarkersSupported();
        bool SetVarjoMarkersEnabled(bool Enabled);
        bool IsVarjoMarkersEnabled();
        bool SetMarkerTrackingTimeout(int32 MarkerId, float Duration);
        bool SetMarkerTrackingMode(int32 MarkerId, EMarkerTrackingMode TrackingMode);
        bool GetMarkerTrackingMode(int32 MarkerId, EMarkerTrackingMode& TrackingMode);

        DECLARE_MULTICAST_DELEGATE_FourParams(FNewVarjoMarkerDetected, int32, const FVector&, const FRotator&, const FVector2D&);
        static FNewVarjoMarkerDetected NewVarjoMarkerDetected;

        DECLARE_MULTICAST_DELEGATE_FourParams(FVarjoMarkerMoved, int32, const FVector&, const FRotator&, const FVector2D&);
        static FVarjoMarkerMoved VarjoMarkerMoved;

        DECLARE_MULTICAST_DELEGATE_OneParam(FVarjoMarkerLost, int32);
        static FVarjoMarkerLost VarjoMarkerLost;


    private:
        bool bVarjoMarkersSupported = false;
        bool bVarjoMarkersEnabled = false;
        PFN_xrSetMarkerTrackingVARJO xrSetMarkerTrackingVARJO = nullptr;
        PFN_xrCreateMarkerSpaceVARJO xrCreateMarkerSpaceVARJO = nullptr;
        PFN_xrGetMarkerSizeVARJO xrGetMarkerSizeVARJO = nullptr;
        PFN_xrSetMarkerTrackingPredictionVARJO xrSetMarkerTrackingPredictionVARJO = nullptr;
        PFN_xrSetMarkerTrackingTimeoutVARJO xrSetMarkerTrackingTimeoutVARJO = nullptr;

        typedef struct  {
            XrSpace space;
            XrExtent2Df size;
            EMarkerTrackingMode trackingMode;
        } Marker;
        TMap<uint64_t, Marker> markers;

        XrSession Session = XR_NULL_HANDLE;
        XrTime displayTime;
        XrSpace trackingSpace;
    };
}
