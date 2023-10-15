//
// Copyright (c) 2023 suzumushi
//
// 2023-10-15		PPprocessor.cpp
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 (CC BY-NC-SA 4.0).
//
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//

#include "PPprocessor.h"
#include "PPcids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
// suzumushi
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include "pluginterfaces/vst/ivstprocesscontext.h"

using namespace Steinberg;

namespace suzumushi {
//------------------------------------------------------------------------
// PolyPortamentoProcessor
//------------------------------------------------------------------------
PolyPortamentoProcessor:: PolyPortamentoProcessor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kPolyPortamentoControllerUID);
}

//------------------------------------------------------------------------
PolyPortamentoProcessor:: ~PolyPortamentoProcessor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API PolyPortamentoProcessor:: initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated
	
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	// if everything Ok, continue
	if (result != kResultOk) {
		return result;
	}

	//--- create Audio IO (for SteinbergTestHost) ------
	// addAudioOutput (STR16 ("unused"), Steinberg::Vst::SpeakerArr::kStereo);

	//--- create MIDI IO ------
	addEventInput (STR16 ("MIDI In"));
	addEventOutput (STR16 ("MIDI Out"));

	processContextRequirements.needTempo ();

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PolyPortamentoProcessor:: terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API PolyPortamentoProcessor:: setActive (TBool state)
{
	// suzumushi:
	if (state != 0)				// if (state == true)
		reset ();

	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API PolyPortamentoProcessor:: process (Vst::ProcessData& data)
{
	//--- First : Read inputs parameter changes-----------

    if (data.inputParameterChanges) {
        int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
        for (int32 index = 0; index < numParamsChanged; index++) {
            if (auto* paramQueue = data.inputParameterChanges->getParameterData (index)) {
                Vst::ParamValue value;
                int32 sampleOffset;
                int32 numPoints = paramQueue->getPointCount ();
				// suzumushi: get the last change
				if (paramQueue->getPoint (numPoints - 1, sampleOffset, value) == kResultTrue)
					gui_param_update (paramQueue->getParameterId (), value);
			}
		}
	}

	if (data.inputEvents != nullptr && data.outputEvents != nullptr) {
		int32 numEvents = data.inputEvents->getEventCount ();
		for (int i = 0; i < numEvents; i++) {
			Vst::Event iEvent;
			if (data.inputEvents->getEvent (i, iEvent) == kResultOk) {
				if (iEvent.type == Vst::Event::kNoteOnEvent && iEvent.noteOn.velocity != 0.0) {
					if (noteOnOff [iEvent.noteOn.channel].voices++ == 0)
						noteOnOff [iEvent.noteOn.channel].on1 = true;
					else
						noteOnOff [iEvent.noteOn.channel].on2 = true;
					noteOnOff [iEvent.noteOn.channel].onSampleOffset = iEvent.sampleOffset;
					noteOnOff [iEvent.noteOn.channel].note = iEvent.noteOn.pitch;
					noteOnOff [iEvent.noteOn.channel].velocity = iEvent.noteOn.velocity;
				} else 
				if (iEvent.type == Vst::Event::kNoteOffEvent || (iEvent.type == Vst::Event::kNoteOnEvent && iEvent.noteOn.velocity == 0.0)) {
					if (noteOnOff [iEvent.noteOff.channel].voices > 0)
						noteOnOff [iEvent.noteOff.channel].voices--;
					if (noteOnOff [iEvent.noteOff.channel].voices == 0) {
						noteOnOff [iEvent.noteOff.channel].off0 = true;
						noteOnOff [iEvent.noteOff.channel].offSampleOffset = iEvent.sampleOffset;
					}
				}
			}
		}
	}

	// update of channel status
	for (int i = 1; i < 16; i++) {
		if (noteOnOff [i].on1) {
			if (noteOnOff [i].off0)	{					// on1 && off0
				if (noteOnOff [i].onSampleOffset == noteOnOff [i].offSampleOffset)
					channelStat [i].cont = true;
				else if (noteOnOff [i].onSampleOffset > noteOnOff [i].offSampleOffset)
					channelStat [i].start = channelStat [i].end = true;
			} else										// on1 && !off0
				channelStat [i].start = true;
		} else {										// !on1
			if (noteOnOff [i].on2) {					// !on1 && on2
				if (noteOnOff [i].off0) {				// !on1 && on2 && off0
					if (noteOnOff [i].onSampleOffset == noteOnOff [i].offSampleOffset)
						channelStat [i].cont = true;
					else if (noteOnOff [i].onSampleOffset < noteOnOff [i].offSampleOffset)
						channelStat [i].end = true;
				} else									// !on1 && on2 && !off0
					channelStat [i].cont = true;
			} else
				if (noteOnOff [i].off0)					// !on1 && !on2 && off0
					channelStat [i].end = true;
		}
		noteOnOff [i].on1 = false;
		noteOnOff [i].on2 = false;
		noteOnOff [i].off0 = false;
	}


	Vst::Event oEvent;
	oEvent.busIndex = 0;
	oEvent.ppqPosition = 0.0;
	oEvent.flags = 0;

	// end of portamento
	oEvent.type = Vst::Event::kNoteOffEvent;
	oEvent.noteOff.pitch = middleNote;
	oEvent.noteOff.velocity = 0;
	for (int i = 1; i < 16; i++) {
		if (channelStat [i].end) {
			channelStat [i].end = false;
			// stop pitch bend output
			channelStat [i].bendCount = 0;				
			channelStat [i].numBend = 0;
			// middle note off
			oEvent.sampleOffset = noteOnOff [i].offSampleOffset;
			oEvent.noteOff.channel = i;
			data.outputEvents->addEvent (oEvent);
		}
	}

	// start of portamento
	// initial pitch bend
	oEvent.type = Vst::Event::kLegacyMIDICCOutEvent;
	oEvent.midiCCOut.controlNumber = Vst::kPitchBend;
	for (int i = 1; i < 16; i++) {
		if (channelStat [i].start) {
			channelStat [i].currentPitch = std::max (std::min (NoteTo.pitch (noteOnOff [i].note) + channelStat [i].pitchBend, 16'383), 0);
			oEvent.sampleOffset = noteOnOff [i].onSampleOffset;
			oEvent.midiCCOut.channel = i;
			oEvent.midiCCOut.value = channelStat [i].currentPitch % 128;		// LSB
			oEvent.midiCCOut.value2 = channelStat [i].currentPitch / 128;		// USB
			data.outputEvents->addEvent (oEvent);
		}
	}
	// middle note on
	oEvent.type = Vst::Event::kNoteOnEvent;
	oEvent.noteOn.pitch = middleNote;
	for (int i = 1; i < 16; i++) {
		if (channelStat [i].start) {
			channelStat [i].start = false;
			oEvent.sampleOffset = noteOnOff [i].onSampleOffset;
			oEvent.noteOn.channel = i;
			oEvent.noteOn.velocity = noteOnOff [i].velocity;
			data.outputEvents->addEvent (oEvent);
		}
	}

	// continue portamento
	if (data.processContext && data.processContext->state & Vst::ProcessContext::kTempoValid)
		tempo = data.processContext->tempo;
	for (int i = 1; i < 16; i++) {
		if (channelStat [i].cont) {
			channelStat [i].cont = false;
			// setup continuous pitch bend information
			channelStat [i].initialPitch = channelStat [i].currentPitch;
			channelStat [i].targetPitch = std::max (std::min (NoteTo.pitch (noteOnOff [i].note) + channelStat [i].pitchBend, 16'383), 0);
			channelStat [i].bendCount = 0;
			channelStat [i].numBend = numBend (i);
			channelStat [i].clock = interval;
		}
	}

	// output pitch bend
	oEvent.type = Vst::Event::kLegacyMIDICCOutEvent;
	oEvent.midiCCOut.controlNumber = Vst::kPitchBend;
	for (int i = 1; i < 16; i++) {
		if (channelStat [i].bendCount < channelStat [i].numBend && (channelStat [i].clock += data.numSamples) >= interval) {
			channelStat [i].bendCount++;
			channelStat [i].clock -= interval;
			if (channelStat [i].bendCount == 1)
				oEvent.sampleOffset = noteOnOff [i].onSampleOffset;
			else
				oEvent.sampleOffset = 0;
			oEvent.midiCCOut.channel = i;
			channelStat [i].currentPitch = currentPitch (i);
			oEvent.midiCCOut.value = channelStat [i].currentPitch % 128;		// LSB
			oEvent.midiCCOut.value2 = channelStat [i].currentPitch / 128;		// USB
			data.outputEvents->addEvent (oEvent);
		}
	}

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PolyPortamentoProcessor:: setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API PolyPortamentoProcessor:: canProcessSampleSize (int32 symbolicSampleSize)
{
	// by default kSample32 is supported
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;

	// disable the following comment if your processing support kSample64
	/* if (symbolicSampleSize == Vst::kSample64)
		return kResultTrue; */

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PolyPortamentoProcessor:: setState (IBStream* state)
{
	// called when we load a preset, the model has to be reloaded
	IBStreamer streamer (state, kLittleEndian);

	// suzumushi:
	int version;
	if (streamer.readInt32 (version) == false)
		return (kResultFalse);

	if (streamer.readDouble (gp.p_time) == false)
		return (kResultFalse);
	if (streamer.readInt32 (gp.time_unit) == false)
		return (kResultFalse);
	if (streamer.readDouble (gp.p_curve) == false)
		return (kResultFalse);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PolyPortamentoProcessor:: getState (IBStream* state)
{
	// here we need to save the model
	IBStreamer streamer (state, kLittleEndian);

	// suzumushi:
	int version = 1;
	if (streamer.writeInt32 (version) == false)
		return (kResultFalse);

	if (streamer.writeDouble (gp.p_time) == false)
		return (kResultFalse);
	if (streamer.writeInt32 (gp.time_unit) == false)
		return (kResultFalse);
	if (streamer.writeDouble (gp.p_curve) == false)
		return (kResultFalse);

	return kResultOk;
}
//------------------------------------------------------------------------
// suzumushi:
void PolyPortamentoProcessor:: gui_param_update (const ParamID paramID, const ParamValue paramValue)
{
	switch (paramID) {
		case p_time.tag:
			gp.p_time = Vst::LogTaperParameter::toPlain (paramValue, p_time);
			break;
		case time_unit.tag:
			gp.time_unit = stringListParameter::toPlain (paramValue, (int32)TIME_UNIT_L::LIST_LEN);
			break;
		case p_curve.tag:
			gp.p_curve = rangeParameter::toPlain (paramValue, p_curve);
			break;
	}
	if (paramID >= PITCH_BEND_02 && paramID <= PITCH_BEND_16)
		channelStat [paramID - PITCH_BEND_01].pitchBend = (int)((paramValue - 0.5) * 16'384.0 + 0.5);
}

int PolyPortamentoProcessor:: currentPitch (const int ch) const
{
	double delta;
	if (gp.p_curve > 0)		// fast start
		delta = 1.0 - std::pow (1.0 - (double)channelStat [ch].bendCount / channelStat [ch].numBend, gp.p_curve + 1.0);
	else					// slow start
		delta = std::pow ((double)channelStat [ch].bendCount / channelStat [ch].numBend, -gp.p_curve + 1.0);	
	return (channelStat [ch].initialPitch + (int) ((channelStat [ch].targetPitch - channelStat [ch].initialPitch) * delta + 0.5));
}

int PolyPortamentoProcessor:: numBend (const int ch) const
{
	double portamentoTime {0.0};		// seconds

	switch (gp.time_unit) {
		case (int32)TIME_UNIT_L::SEC:
			portamentoTime = gp.p_time;
			break;
		case (int32)TIME_UNIT_L::SECOCT:
			portamentoTime = (gp.p_time / 12.0) * abs (channelStat [ch].targetPitch - channelStat [ch].initialPitch) * sensitivity / 8'192.0;
			break;
		case (int32)TIME_UNIT_L::BEAT:
			portamentoTime = gp.p_time * (60.0 / tempo);
			break;
		case (int32)TIME_UNIT_L::BEATOCT:
			portamentoTime = (gp.p_time / 12.0) * (60.0 / tempo) * abs (channelStat [ch].targetPitch - channelStat [ch].initialPitch) * sensitivity / 8'192.0;
			break;
	}
	return ((int)(portamentoTime * pitchBendCycle + 0.5) + 1);
}

void PolyPortamentoProcessor:: reset ()
{
	interval = processSetup.sampleRate / pitchBendCycle + 0.5;
	for (int i = 0; i < 16; i++) {
		noteOnOff [i].on1 = false;
		noteOnOff [i].on2 = false;
		noteOnOff [i].off0 = false;
		noteOnOff [i].voices = 0;
		channelStat [i].start = false;
		channelStat [i].end = false;
		channelStat [i].cont = false;
		channelStat [i].pitchBend = 0;
		channelStat [i].bendCount = 0;
		channelStat [i].numBend = 0;
	}
}

//------------------------------------------------------------------------
} // namespace suzumushi
