//
// Copyright (c) 2023 suzumushi
//
// 2023-8-18		PPcontroller.h
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 (CC BY-NC-SA 4.0).
//
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//

#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"

// suzumushi: 
#include "PPparam.h"

namespace suzumushi {

//------------------------------------------------------------------------
//  PolyPortamentoController
// 
// 	suzumushi: see /VST Home/Technical Documentation/[3.0.1] Parameter MIDI Mapping
//------------------------------------------------------------------------
class PolyPortamentoController : public Steinberg::Vst::EditControllerEx1,  public Steinberg::Vst::IMidiMapping		// suzumushi
{
public:
//------------------------------------------------------------------------
	PolyPortamentoController () = default;
	~PolyPortamentoController () SMTG_OVERRIDE = default;

    // Create function
	static Steinberg::FUnknown* createInstance (void* /*context*/)
	{
		return (Steinberg::Vst::IEditController*)new PolyPortamentoController;
	}

	// IPluginBase
	Steinberg::tresult PLUGIN_API initialize (Steinberg::FUnknown* context) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API terminate () SMTG_OVERRIDE;

	// EditController
	Steinberg::tresult PLUGIN_API setComponentState (Steinberg::IBStream* state) SMTG_OVERRIDE;
	Steinberg::IPlugView* PLUGIN_API createView (Steinberg::FIDString name) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API setState (Steinberg::IBStream* state) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API getState (Steinberg::IBStream* state) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API setParamNormalized (Steinberg::Vst::ParamID tag,
                                                      Steinberg::Vst::ParamValue value) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API getParamStringByValue (Steinberg::Vst::ParamID tag,
                                                         Steinberg::Vst::ParamValue valueNormalized,
                                                         Steinberg::Vst::String128 string) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API getParamValueByString (Steinberg::Vst::ParamID tag,
                                                         Steinberg::Vst::TChar* string,
                                                         Steinberg::Vst::ParamValue& valueNormalized) SMTG_OVERRIDE;
	// suzumushi
	//---IMidiMapping---------------------------
	Steinberg::tresult PLUGIN_API getMidiControllerAssignment (Steinberg::int32 busIndex, Steinberg::int16 channel, 
														Steinberg::Vst::CtrlNumber midiControllerNumber, 
														Steinberg::Vst::ParamID& id) SMTG_OVERRIDE;
	//---Interface---------
	// suzumushi
	OBJ_METHODS (PolyPortamentoController, EditControllerEx1)
	DEFINE_INTERFACES
		// suzumushi
		DEF_INTERFACE (IMidiMapping)
	END_DEFINE_INTERFACES (EditController)
    DELEGATE_REFCOUNT (EditController)

//------------------------------------------------------------------------
protected:
};

//------------------------------------------------------------------------
} // namespace suzumushi
