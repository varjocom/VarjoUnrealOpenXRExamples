// Copyright 2021 Varjo Technologies Oy. All rights reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "VarjoOpenXR.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogVarjoOpenXR, Log, All);

UENUM(BlueprintType, Category = "VarjoOpenXR|Markers")
enum class EMarkerTrackingMode : uint8
{
    Stationary = 0,
    Dynamic = 1
};

UCLASS(ClassGroup = OpenXR)
class VARJOOPENXR_API UVarjoOpenXRFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    /**
     * Enables/disables depth testing.
     *
     * @param    Enabled
     */
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Depth Test Enabled", Keywords = "VarjoOpenXR depth test"), Category = "VarjoOpenXR|Depth")
        static void SetDepthTestEnabled(bool Enabled);

    /**
     * Check if depth testing is supported by active OpenXR runtime.
     */
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Is Depth Test Supported", Keywords = "VarjoOpenXR depth test"), Category = "VarjoOpenXR|Depth")
        static bool IsDepthTestSupported();

    /**
     * Check if depth testing is enabled.
     */
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Is Depth Test Enabled", Keywords = "VarjoOpenXR depth test"), Category = "VarjoOpenXR|Depth")
        static bool IsDepthTestEnabled();

    /**
     * Sets the depth test range.
     *
     * @param    Enabled
     * @param    NearZ is a non-negative distance in meters that specifies the lower bound of the range where depth testing should be performed. Must be less than depthTestRangeFarZ.
     * @param    FarZ is a positive distance in meters that specifies the upper bound of the range where depth testing should be performed. Must be greater than depthTestRangeNearZ.
     */
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Depth Test Range", Keywords = "VarjoOpenXR depth test range"), Category = "VarjoOpenXR|Depth")
        static void SetDepthTestRange(bool Enabled, float NearZ = 0.0f, float FarZ = 1.0f);

    /**
     * Get depth test range.
     */
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Depth Test Range", Keywords = "VarjoOpenXR depth test range"), Category = "VarjoOpenXR|Depth")
        static void GetDepthTestRange(bool& bIsEnabled, float& NearZ, float& FarZ);

    /**
     * Enables/disables environment depth estimation.
     *
     * @param    Enabled
     */
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Environment Depth Estimation Enabled", Keywords = "VarjoOpenXR environment depth estimation"), Category = "VarjoOpenXR|Depth")
        static void SetEnvironmentDepthEstimationEnabled(bool Enabled);

    /**
     * Check if environment depth estimation is supported by active OpenXR runtime.
     */
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Is Environment Depth Estimation Supported", Keywords = "VarjoOpenXR environment depth estimation"), Category = "VarjoOpenXR|Depth")
        static bool IsEnvironmentDepthEstimationSupported();

    /**
     * Check if environment depth estimation is enabled.
     */
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Is Environment Depth Estimation Enabled", Keywords = "VarjoOpenXR environment depth estimation"), Category = "VarjoOpenXR|Depth")
        static bool IsEnvironmentDepthEstimationEnabled();

    /**
     * Check if foveated rendering is supported by active OpenXR runtime.
     */
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Is Foveated Rendering Supported", Keywords = "VarjoOpenXR foveated rendering"), Category = "VarjoOpenXR|Foveated Rendering")
        static bool IsFoveatedRenderingSupported();

    /**
     * Check if foveated rendering is enabled.
     */
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Is Foveated Rendering Enabled", Keywords = "VarjoOpenXR foveated rendering"), Category = "VarjoOpenXR|Foveated Rendering")
        static bool IsFoveatedRenderingEnabled();

    /**
     * Check if Varjo Marker tracking is supported.
     */
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Is Varjo Marker Tracking Supported", Keywords = "VarjoOpenXR Marker Tracking"), Category = "VarjoOpenXR|Markers")
        static bool IsVarjoMarkersSupported();

    /**
     * Enables/disables Varjo Marker tracking.
     *
     * @param    Enabled
     * @return   True if Varjo Marker tracking is enabled. This call will fail if HMD is not capable of tracking markers.
     */
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Varjo Marker Tracking Enabled", Keywords = "VarjoOpenXR Marker Tracking"), Category = "VarjoOpenXR|Markers")
        static bool SetVarjoMarkersEnabled(bool Enabled);

    /**
     * Check if Varjo Marker Tracking is enabled.
     */
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Is Varjo Marker Tracking Enabled", Keywords = "VarjoOpenXR Marker Tracking"), Category = "VarjoOpenXR|Markers")
        static bool IsVarjoMarkersEnabled();

    /**
     * Set timeout on a marker
     * @param    MarkerId
     * @param    Duration in seconds.
     * @return   True if marker timeout was set successfully.
     */
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Marker Timeout (Default is ZERO)", Keywords = "VarjoOpenXR Marker Tracking"), Category = "VarjoOpenXR|Markers")
        static bool SetMarkerTimeout(int32 MarkerId, float Duration);

    /**
     * Set marker tracking mode. Use Dynamic for more responsive pose updates according to marker's movements. By default markers are treated as Stationary - less responsive to marker's movements.
     * @param    MarkerId
     * @param    TrackingMode
     * @return   True if marker tracking mode was set successfully.
     */
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Marker Tracking Mode", Keywords = "VarjoOpenXR Marker Tracking"), Category = "VarjoOpenXR|Markers")
        static bool SetMarkerTrackingMode(int32 MarkerId, EMarkerTrackingMode TrackingMode);


    /**
     * Get marker tracking mode.
     * @param    MarkerId
     * @param    TrackingMode
     * @return   True if valid MarkerId is given.
     */
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Marker Tracking Mode", Keywords = "VarjoOpenXR Marker Tracking"), Category = "VarjoOpenXR|Markers")
        static bool GetMarkerTrackingMode(int32 MarkerId, EMarkerTrackingMode& TrackingMode);
};