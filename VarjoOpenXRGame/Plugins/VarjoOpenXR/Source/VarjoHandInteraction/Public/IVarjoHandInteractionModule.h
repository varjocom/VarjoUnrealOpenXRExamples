// Copyright 2025 Varjo Technologies Oy. All rights reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "IInputDeviceModule.h"

class IVarjoHandInteractionModule : public IInputDeviceModule
{
public:
	static FName VarjoHandInteractionModularKeyName;

	static FName GetModuleKeyName()
	{
		return VarjoHandInteractionModularKeyName;
	}
};
