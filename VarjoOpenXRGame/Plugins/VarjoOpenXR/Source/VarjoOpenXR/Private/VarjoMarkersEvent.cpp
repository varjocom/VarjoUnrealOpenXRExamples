// Copyright 2021 Varjo Technologies Oy. All rights reserved.

#include "VarjoMarkersEvent.h"

#include "Misc/CoreDelegates.h"


UVarjoMarkerDelegates::FNewVarjoMarkerDetected UVarjoMarkerDelegates::NewVarjoMarkerDetected;
UVarjoMarkerDelegates::FVarjoMarkerMoved UVarjoMarkerDelegates::VarjoMarkerMoved;
UVarjoMarkerDelegates::FVarjoMarkerLost UVarjoMarkerDelegates::VarjoMarkerLost;

void UVarjoMarkersEvent::OnRegister()
{
    Super::OnRegister();

    UVarjoMarkerDelegates::NewVarjoMarkerDetected.AddUObject(this, &UVarjoMarkersEvent::FNewVarjoMarkerDetected_Handler);
    UVarjoMarkerDelegates::VarjoMarkerMoved.AddUObject(this, &UVarjoMarkersEvent::FVarjoMarkerMoved_Handler);
    UVarjoMarkerDelegates::VarjoMarkerLost.AddUObject(this, &UVarjoMarkersEvent::FVarjoMarkerLost_Handler);
}

void UVarjoMarkersEvent::OnUnregister()
{
    Super::OnUnregister();

    UVarjoMarkerDelegates::NewVarjoMarkerDetected.RemoveAll(this);
    UVarjoMarkerDelegates::VarjoMarkerMoved.RemoveAll(this);
    UVarjoMarkerDelegates::VarjoMarkerLost.RemoveAll(this);
}