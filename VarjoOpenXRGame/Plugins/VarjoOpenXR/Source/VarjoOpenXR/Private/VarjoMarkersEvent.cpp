// Copyright 2021 Varjo Technologies Oy. All rights reserved.

#include "VarjoMarkersEvent.h"

#include "Misc/CoreDelegates.h"
#include "VarjoMarkersPlugin.h"

void UVarjoMarkersEvent::OnRegister()
{
    Super::OnRegister();

    VarjoOpenXR::FVarjoMarkersPlugin::NewVarjoMarkerDetected.AddUObject(this, &UVarjoMarkersEvent::FNewVarjoMarkerDetected_Handler);
    VarjoOpenXR::FVarjoMarkersPlugin::VarjoMarkerMoved.AddUObject(this, &UVarjoMarkersEvent::FVarjoMarkerMoved_Handler);
    VarjoOpenXR::FVarjoMarkersPlugin::VarjoMarkerLost.AddUObject(this, &UVarjoMarkersEvent::FVarjoMarkerLost_Handler);
}

void UVarjoMarkersEvent::OnUnregister()
{
    Super::OnUnregister();

    VarjoOpenXR::FVarjoMarkersPlugin::NewVarjoMarkerDetected.RemoveAll(this);
    VarjoOpenXR::FVarjoMarkersPlugin::VarjoMarkerMoved.RemoveAll(this);
    VarjoOpenXR::FVarjoMarkersPlugin::VarjoMarkerLost.RemoveAll(this);
}