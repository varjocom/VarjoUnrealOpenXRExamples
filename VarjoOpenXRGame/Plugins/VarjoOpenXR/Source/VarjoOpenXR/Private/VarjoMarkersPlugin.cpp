// Copyright 2021 Varjo Technologies Oy. All rights reserved.

#include "VarjoMarkersPlugin.h"
#include "VarjoOpenXR.h"

#include "UObject/NameTypes.h"
#include "IXRTrackingSystem.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"


namespace VarjoOpenXR
{
    FVarjoMarkersPlugin::FNewVarjoMarkerDetected FVarjoMarkersPlugin::NewVarjoMarkerDetected;
    FVarjoMarkersPlugin::FVarjoMarkerMoved FVarjoMarkersPlugin::VarjoMarkerMoved;
    FVarjoMarkersPlugin::FVarjoMarkerLost FVarjoMarkersPlugin::VarjoMarkerLost;

    void FVarjoMarkersPlugin::Register()
    {
        RegisterOpenXRExtensionModularFeature();
    }

    void FVarjoMarkersPlugin::Unregister()
    {
        UnregisterOpenXRExtensionModularFeature();
    }

    bool FVarjoMarkersPlugin::GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions)
    {
        OutExtensions.Add(XR_VARJO_MARKER_TRACKING_EXTENSION_NAME);

        return true;
    }

    void FVarjoMarkersPlugin::PostGetSystem(XrInstance InInstance, XrSystemId InSystem)
    {
        XrSystemMarkerTrackingPropertiesVARJO VarjoMarkersProperties{ XR_TYPE_SYSTEM_MARKER_TRACKING_PROPERTIES_VARJO, nullptr };
        XrSystemProperties SystemProperties{ XR_TYPE_SYSTEM_PROPERTIES, &VarjoMarkersProperties };
        XR_ENSURE(xrGetSystemProperties(InInstance, InSystem, &SystemProperties));

        bVarjoMarkersSupported = VarjoMarkersProperties.supportsMarkerTracking == XR_TRUE;
    }

    const void* FVarjoMarkersPlugin::OnCreateSession(XrInstance InInstance, XrSystemId InSystem, const void* InNext)
    {
        XR_ENSURE(xrGetInstanceProcAddr(InInstance, "xrSetMarkerTrackingVARJO", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetMarkerTrackingVARJO)));
        XR_ENSURE(xrGetInstanceProcAddr(InInstance, "xrCreateMarkerSpaceVARJO", reinterpret_cast<PFN_xrVoidFunction*>(&xrCreateMarkerSpaceVARJO)));
        XR_ENSURE(xrGetInstanceProcAddr(InInstance, "xrGetMarkerSizeVARJO", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetMarkerSizeVARJO)));
        XR_ENSURE(xrGetInstanceProcAddr(InInstance, "xrSetMarkerTrackingPredictionVARJO", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetMarkerTrackingPredictionVARJO)));
        XR_ENSURE(xrGetInstanceProcAddr(InInstance, "xrSetMarkerTrackingTimeoutVARJO", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetMarkerTrackingTimeoutVARJO)));

        return InNext;
    }

    void FVarjoMarkersPlugin::PostCreateSession(XrSession InSession)
    {
        bVarjoMarkersEnabled = false;
        markers.Reset();
        Session = InSession;
    }

    void FVarjoMarkersPlugin::OnEvent(XrSession InSession, const XrEventDataBaseHeader* InHeader)
    {
        if (InHeader->type == XR_TYPE_EVENT_DATA_MARKER_TRACKING_UPDATE_VARJO)
        {
            bool newMarker = false;
            const auto& markerUpdate = *reinterpret_cast<const XrEventDataMarkerTrackingUpdateVARJO*>(InHeader);
            auto id = markerUpdate.markerId;
            if (!markers.Contains(id) && markerUpdate.isActive) {
                XrMarkerSpaceCreateInfoVARJO spaceInfo;
                spaceInfo.type = XR_TYPE_MARKER_SPACE_CREATE_INFO_VARJO;
                spaceInfo.markerId = id;
                spaceInfo.poseInMarkerSpace = XrPosef{ 0 };
                spaceInfo.poseInMarkerSpace.orientation.w = 1.0f;
                Marker marker;

                XR_ENSURE(xrCreateMarkerSpaceVARJO(Session, &spaceInfo, &marker.space));
                XR_ENSURE(xrGetMarkerSizeVARJO(Session, id, &marker.size));

                newMarker = true;
                markers.Add(id, marker);
            }

            if (markerUpdate.isActive)
            {
                markers[id].trackingMode = markerUpdate.isPredicted ? EMarkerTrackingMode::Dynamic : EMarkerTrackingMode::Stationary;
                XrSpaceLocation location{ XR_TYPE_SPACE_LOCATION };

                XR_ENSURE(xrLocateSpace(markers[id].space, trackingSpace, displayTime, &location));
                if (location.locationFlags & XR_SPACE_LOCATION_POSITION_TRACKED_BIT)
                {
                    FTransform markerTransform{};
                    const XrPosef& pose = location.pose;
                    const float worldToMetersScale = GEngine->XRSystem->GetWorldToMetersScale();
                    const FTransform trackerTransform = ToFTransform(pose, worldToMetersScale);
                    const FTransform& trackingToWoldTransform = GEngine->XRSystem->GetTrackingToWorldTransform();
                    const FVector2D size = { markers[id].size.width * worldToMetersScale, markers[id].size.height * worldToMetersScale };
                    markerTransform = trackerTransform * trackingToWoldTransform;

                    if (newMarker) {
                        NewVarjoMarkerDetected.Broadcast(id, markerTransform.GetLocation(), markerTransform.Rotator(), size);
                    }
                    else {
                        VarjoMarkerMoved.Broadcast(id, markerTransform.GetLocation(), markerTransform.Rotator(), size);
                    }
                }
            }
            else
            {
                VarjoMarkerLost.Broadcast(id);
            }
        }
    }

    void FVarjoMarkersPlugin::UpdateDeviceLocations(XrSession InSession, XrTime DisplayTime, XrSpace TrackingSpace)
    {
        displayTime = DisplayTime;
        trackingSpace = TrackingSpace;
    }

    bool FVarjoMarkersPlugin::IsVarjoMarkersSupported()
    {
        return bVarjoMarkersSupported;
    }

    bool FVarjoMarkersPlugin::SetVarjoMarkersEnabled(bool Enabled)
    {
        if (xrSetMarkerTrackingVARJO == nullptr || bVarjoMarkersSupported == false)
        {
            bVarjoMarkersEnabled = false;
        }
        else
        {
            bVarjoMarkersEnabled = XR_ENSURE(xrSetMarkerTrackingVARJO(Session, Enabled)) && Enabled;
            if (!bVarjoMarkersEnabled) {
                markers.Reset();
            }
        }
        return bVarjoMarkersEnabled;
    }

    bool FVarjoMarkersPlugin::IsVarjoMarkersEnabled()
    {
        return bVarjoMarkersEnabled;
    }

    bool FVarjoMarkersPlugin::SetMarkerTrackingTimeout(int32 MarkerId, float Duration)
    {
        if (MarkerId == 0 || !markers.Contains(MarkerId))
        {
            UE_LOG(LogVarjoOpenXR, Warning, TEXT("SetMarkerTrackingTimeout called with invalid Marker ID %d. Ignoring."), MarkerId);
            return false;
        }

        XrDuration duration = Duration * 1000000000.;
        if (XR_SUCCESS != xrSetMarkerTrackingTimeoutVARJO(Session, MarkerId, duration)) {
            UE_LOG(LogVarjoOpenXR, Error, TEXT("SetMarkerTrackingTimeout failed for Marker ID %d."), MarkerId);
            return false;
        }

        return true;
    }

    bool FVarjoMarkersPlugin::SetMarkerTrackingMode(int32 MarkerId, EMarkerTrackingMode TrackingMode)
    {
        if (MarkerId == 0 || !markers.Contains(MarkerId))
        {
            UE_LOG(LogVarjoOpenXR, Warning, TEXT("SetMarkerTrackingMode called with invalid Marker ID %d. Ignoring."), MarkerId);
            return false;
        }

        if (XR_SUCCESS != xrSetMarkerTrackingPredictionVARJO(Session, MarkerId, TrackingMode != EMarkerTrackingMode::Stationary)) {
            UE_LOG(LogVarjoOpenXR, Error, TEXT("SetMarkerTrackingMode failed for Marker ID %d."), MarkerId);
            return false;
        }

        markers[MarkerId].trackingMode = TrackingMode;

        return true;
    }

    bool FVarjoMarkersPlugin::GetMarkerTrackingMode(int32 MarkerId, EMarkerTrackingMode& TrackingMode)
    {
        if (MarkerId == 0 || !markers.Contains(MarkerId))
        {
            TrackingMode = EMarkerTrackingMode::Stationary;
            UE_LOG(LogVarjoOpenXR, Warning, TEXT("GetMarkerTrackingMode called with invalid Marker ID %d."), MarkerId);
            return false;
        }

        TrackingMode = markers[MarkerId].trackingMode;
        return true;
    }
}