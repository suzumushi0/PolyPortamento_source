//
// Copyright (c) 2023 suzumushi
//
// 2023-8-18		PPcids.h
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 (CC BY-NC-SA 4.0).
//
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace suzumushi {
//------------------------------------------------------------------------
static const Steinberg::FUID kPolyPortamentoProcessorUID (0x4CCF5BB3, 0x59C7586B, 0x9C0046EF, 0xA69EED29);
static const Steinberg::FUID kPolyPortamentoControllerUID (0x1D78D664, 0x08F75686, 0xB53642A3, 0xB7E33E24);

#define PolyPortamentoVST3Category "Instrument"

//------------------------------------------------------------------------
} // namespace suzumushi
