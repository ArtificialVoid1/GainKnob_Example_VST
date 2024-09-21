//------------------------------------------------------------------------
// Copyright(c) 2024 ArtificialVoid1.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace MyCompanyName {
//------------------------------------------------------------------------
static const Steinberg::FUID kGain_KnobProcessorUID (0x1148CF49, 0xEC2F59D5, 0x97294578, 0x045D3467);
static const Steinberg::FUID kGain_KnobControllerUID (0x3FD0B45E, 0x3D675F48, 0x9C291EC3, 0xF5D236E8);

#define Gain_KnobVST3Category "Fx"

enum params {
	kGain = 100,
};

//------------------------------------------------------------------------
} // namespace MyCompanyName
