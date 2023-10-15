//
// Copyright (c) 2023 suzumushi
//
// 2023-10-11		PPcontroller.cpp
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 (CC BY-NC-SA 4.0).
//
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//

#include "PPcontroller.h"
#include "PPcids.h"
#include "vstgui/plugin-bindings/vst3editor.h"

// suzumushi:
#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"

using namespace Steinberg;

namespace suzumushi {

//------------------------------------------------------------------------
// PolyPortamentoController Implementation
//------------------------------------------------------------------------
tresult PLUGIN_API PolyPortamentoController:: initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated

	//---do not forget to call parent ------
	tresult result = EditControllerEx1::initialize (context);
	if (result != kResultOk) {
		return result;
	}

	// Here you could register some parameters

	// suzumushi: registration of GUI and host facing parameters 

	Vst::LogTaperParameter* p_time_param = new Vst::LogTaperParameter (
		STR16 ("Portamento time"), p_time.tag, STR16 (""),
		p_time.min, p_time.max, p_time.def, p_time.steps, p_time.flags);
	p_time_param -> setPrecision (precision2);
	parameters.addParameter (p_time_param);

	Vst::StringListParameter* time_unit_param = new Vst::StringListParameter (
		STR16 ("Units of portamento time"), time_unit.tag, nullptr, time_unit.flags);
	time_unit_param -> appendString (STR16 ("seconds"));
	time_unit_param -> appendString (STR16 ("seconds / octave"));
	time_unit_param -> appendString (STR16 ("beats (quarter notes)"));
	time_unit_param -> appendString (STR16 ("beats / octave"));
	parameters.addParameter (time_unit_param);

	Vst::RangeParameter* p_curve_param = new Vst::RangeParameter (
		STR16 ("Portamento curve"), p_curve.tag, STR16 (""),
		p_curve.min, p_curve.max, p_curve.def, p_curve.steps, p_curve.flags);
	p_curve_param -> setPrecision (precision2);
	parameters.addParameter (p_curve_param);

	Vst::RangeParameter* pitch_bend_param_02 = new Vst::RangeParameter (
		STR16 ("Ch 2 pitch bend"), PITCH_BEND_02, STR16 ("Semitone"),
		pitch_bend.min, pitch_bend.max, pitch_bend.def, pitch_bend.steps, pitch_bend.flags);
	pitch_bend_param_02 -> setPrecision (precision2);
	parameters.addParameter (pitch_bend_param_02);

	Vst::RangeParameter* pitch_bend_param_03 = new Vst::RangeParameter (
		STR16 ("Ch 3 pitch bend"), PITCH_BEND_03, STR16 ("Semitone"),
		pitch_bend.min, pitch_bend.max, pitch_bend.def, pitch_bend.steps, pitch_bend.flags);
	pitch_bend_param_03 -> setPrecision (precision2);
	parameters.addParameter (pitch_bend_param_03);

	Vst::RangeParameter* pitch_bend_param_04 = new Vst::RangeParameter (
		STR16 ("Ch 4 pitch bend"), PITCH_BEND_04, STR16 ("Semitone"),
		pitch_bend.min, pitch_bend.max, pitch_bend.def, pitch_bend.steps, pitch_bend.flags);
	pitch_bend_param_04 -> setPrecision (precision2);
	parameters.addParameter (pitch_bend_param_04);

	Vst::RangeParameter* pitch_bend_param_05 = new Vst::RangeParameter (
		STR16 ("Ch 5 pitch bend"), PITCH_BEND_05, STR16 ("Semitone"),
		pitch_bend.min, pitch_bend.max, pitch_bend.def, pitch_bend.steps, pitch_bend.flags);
	pitch_bend_param_05 -> setPrecision (precision2);
	parameters.addParameter (pitch_bend_param_05);

	Vst::RangeParameter* pitch_bend_param_06 = new Vst::RangeParameter (
		STR16 ("Ch 6 pitch bend"), PITCH_BEND_06, STR16 ("Semitone"),
		pitch_bend.min, pitch_bend.max, pitch_bend.def, pitch_bend.steps, pitch_bend.flags);
	pitch_bend_param_06 -> setPrecision (precision2);
	parameters.addParameter (pitch_bend_param_06);

	Vst::RangeParameter* pitch_bend_param_07 = new Vst::RangeParameter (
		STR16 ("Ch 7 pitch bend"), PITCH_BEND_07, STR16 ("Semitone"),
		pitch_bend.min, pitch_bend.max, pitch_bend.def, pitch_bend.steps, pitch_bend.flags);
	pitch_bend_param_07 -> setPrecision (precision2);
	parameters.addParameter (pitch_bend_param_07);

	Vst::RangeParameter* pitch_bend_param_08 = new Vst::RangeParameter (
		STR16 ("Ch 8 pitch bend"), PITCH_BEND_08, STR16 ("Semitone"),
		pitch_bend.min, pitch_bend.max, pitch_bend.def, pitch_bend.steps, pitch_bend.flags);
	pitch_bend_param_08 -> setPrecision (precision2);
	parameters.addParameter (pitch_bend_param_08);

	Vst::RangeParameter* pitch_bend_param_09 = new Vst::RangeParameter (
		STR16 ("Ch 9 pitch bend"), PITCH_BEND_09, STR16 ("Semitone"),
		pitch_bend.min, pitch_bend.max, pitch_bend.def, pitch_bend.steps, pitch_bend.flags);
	pitch_bend_param_09 -> setPrecision (precision2);
	parameters.addParameter (pitch_bend_param_09);

	Vst::RangeParameter* pitch_bend_param_10 = new Vst::RangeParameter (
		STR16 ("Ch 10 pitch bend"), PITCH_BEND_10, STR16 ("Semitone"),
		pitch_bend.min, pitch_bend.max, pitch_bend.def, pitch_bend.steps, pitch_bend.flags);
	pitch_bend_param_10 -> setPrecision (precision2);
	parameters.addParameter (pitch_bend_param_10);

	Vst::RangeParameter* pitch_bend_param_11 = new Vst::RangeParameter (
		STR16 ("Ch 11 pitch bend"), PITCH_BEND_11, STR16 ("Semitone"),
		pitch_bend.min, pitch_bend.max, pitch_bend.def, pitch_bend.steps, pitch_bend.flags);
	pitch_bend_param_11 -> setPrecision (precision2);
	parameters.addParameter (pitch_bend_param_11);

	Vst::RangeParameter* pitch_bend_param_12 = new Vst::RangeParameter (
		STR16 ("Ch 12 pitch bend"), PITCH_BEND_12, STR16 ("Semitone"),
		pitch_bend.min, pitch_bend.max, pitch_bend.def, pitch_bend.steps, pitch_bend.flags);
	pitch_bend_param_12 -> setPrecision (precision2);
	parameters.addParameter (pitch_bend_param_12);

	Vst::RangeParameter* pitch_bend_param_13 = new Vst::RangeParameter (
		STR16 ("Ch 13 pitch bend"), PITCH_BEND_13, STR16 ("Semitone"),
		pitch_bend.min, pitch_bend.max, pitch_bend.def, pitch_bend.steps, pitch_bend.flags);
	pitch_bend_param_13 -> setPrecision (precision2);
	parameters.addParameter (pitch_bend_param_13);

	Vst::RangeParameter* pitch_bend_param_14 = new Vst::RangeParameter (
		STR16 ("Ch 14 pitch bend"), PITCH_BEND_14, STR16 ("Semitone"),
		pitch_bend.min, pitch_bend.max, pitch_bend.def, pitch_bend.steps, pitch_bend.flags);
	pitch_bend_param_14 -> setPrecision (precision2);
	parameters.addParameter (pitch_bend_param_14);

	Vst::RangeParameter* pitch_bend_param_15 = new Vst::RangeParameter (
		STR16 ("Ch 15 pitch bend"), PITCH_BEND_15, STR16 ("Semitone"),
		pitch_bend.min, pitch_bend.max, pitch_bend.def, pitch_bend.steps, pitch_bend.flags);
	pitch_bend_param_15 -> setPrecision (precision2);
	parameters.addParameter (pitch_bend_param_15);

	Vst::RangeParameter* pitch_bend_param_16 = new Vst::RangeParameter (
		STR16 ("Ch 16 pitch bend"), PITCH_BEND_16, STR16 ("Semitone"),
		pitch_bend.min, pitch_bend.max, pitch_bend.def, pitch_bend.steps, pitch_bend.flags);
	pitch_bend_param_16 -> setPrecision (precision2);
	parameters.addParameter (pitch_bend_param_16);

	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PolyPortamentoController:: terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!

	//---do not forget to call parent ------
	return EditControllerEx1::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API PolyPortamentoController:: setComponentState (IBStream* state)
{
	// Here you get the state of the component (Processor part)
	if (!state)
		return kResultFalse;

	// suzumushi:
	ParamValue dtmp;
	int32 itmp;
	int32 version;
	IBStreamer streamer (state, kLittleEndian);

	if (streamer.readInt32 (version) == false)
		return (kResultFalse);

	if (streamer.readDouble (dtmp) == false)
		return (kResultFalse);
	setParamNormalized (p_time.tag, plainParamToNormalized (p_time.tag, dtmp));

	if (streamer.readInt32 (itmp) == false)
		return (kResultFalse);
	setParamNormalized (time_unit.tag, plainParamToNormalized (time_unit.tag, (ParamValue)itmp));

	if (streamer.readDouble (dtmp) == false)
		return (kResultFalse);
	setParamNormalized (p_curve.tag, plainParamToNormalized (p_curve.tag, dtmp));

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PolyPortamentoController:: setState (IBStream* state)
{
	// Here you get the state of the controller

	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PolyPortamentoController:: getState (IBStream* state)
{
	// Here you are asked to deliver the state of the controller (if needed)
	// Note: the real state of your plug-in is saved in the processor

	return kResultTrue;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API PolyPortamentoController:: createView (FIDString name)
{
	// Here the Host wants to open your editor (if you have one)
	if (FIDStringsEqual (name, Vst::ViewType::kEditor))
	{
		// create your editor here and return a IPlugView ptr of it
		auto* view = new VSTGUI::VST3Editor (this, "view", "PolyPortamento.uidesc");

		// suzumushi
		std::vector<double> zoom_factors = {0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0};
		view->setAllowedZoomFactors (zoom_factors);

		return view;
	}
	return nullptr;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PolyPortamentoController:: setParamNormalized (Vst::ParamID tag, Vst::ParamValue value)
{
	// called by host to update your parameters
	tresult result = EditControllerEx1::setParamNormalized (tag, value);
	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PolyPortamentoController:: getParamStringByValue (Vst::ParamID tag, Vst::ParamValue valueNormalized, Vst::String128 string)
{
	// called by host to get a string for given normalized value of a specific parameter
	// (without having to set the value!)
	return EditControllerEx1::getParamStringByValue (tag, valueNormalized, string);
}

//------------------------------------------------------------------------
tresult PLUGIN_API PolyPortamentoController:: getParamValueByString (Vst::ParamID tag, Vst::TChar* string, Vst::ParamValue& valueNormalized)
{
	// called by host to get a normalized value from a string representation of a specific parameter
	// (without having to set the value!)
	return EditControllerEx1::getParamValueByString (tag, string, valueNormalized);
}

//--------------------------------------
tresult PLUGIN_API PolyPortamentoController:: getMidiControllerAssignment (Steinberg::int32 busIndex, Steinberg::int16 channel, Steinberg::Vst::CtrlNumber midiControllerNumber, Steinberg::Vst::ParamID& id)
{
	if (busIndex == 0 && (channel > 0 && channel < 16) && midiControllerNumber == Steinberg::Vst::kPitchBend) {
		id = PITCH_BEND_01 + channel;
		return kResultTrue;
	}
	return kResultFalse;
}

//------------------------------------------------------------------------
} // namespace suzumushi
