// Copyright 2021 Varjo Technologies Oy. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Engine/EngineTypes.h"

DECLARE_LOG_CATEGORY_EXTERN(LogVarjoOpenXRSettings, Log, All);

#include "VarjoOpenXRRuntimeSettings.generated.h"


UENUM()
enum VarjoRenderingMode : int
{
    VarjoRenderingMode_QuadView     UMETA(DisplayName = "Quad View"),
    VarjoRenderingMode_Stereo       UMETA(DisplayName = "Stereo"),
};

UCLASS(config = Engine, defaultconfig)
class VARJOOPENXRRUNTIMESETTINGS_API UVarjoOpenXRRuntimeSettings : public UObject
{
public:
    GENERATED_BODY()

    UPROPERTY(GlobalConfig, EditAnywhere, Category = "Rendering Mode", Meta = (DisplayName = "Rendering Mode", Tooltip = "Please note that Quad View Rendering is usually required for high performance, high quality rendering, but Stereo Rendering might be better supported with some rendering features."))
        TEnumAsByte<VarjoRenderingMode> RenderingMode = VarjoRenderingMode_QuadView;
    UPROPERTY(GlobalConfig, EditAnywhere, Category = "Foveated Rendering", Meta = (DisplayName = "Enable Foveated Rendering", Tooltip = "Enable Foveated Rendering. Please note that this option is usually required for high performance, high quality rendering."))
        bool FoveatedRendering = true;

#if WITH_EDITOR
    virtual bool CanEditChange(const FProperty* InProperty) const override;

    IConsoleVariable* CVarReferenceOpenXRPreferredViewConfiguration;
    virtual void PostInitProperties() override;
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
};
