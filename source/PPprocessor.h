//
// Copyright (c) 2023 suzumushi
//
// 2023-9-2		PPprocessor.h
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 (CC BY-NC-SA 4.0).
//
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//

#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "pluginterfaces/vst/ivstevents.h"

// suzumushi:
#include "PPparam.h"
#include "PPpitch.h"

namespace suzumushi {

constexpr int  pitchBendCycle {60};						// pitch bend message transmission cycle [Hz]

//------------------------------------------------------------------------
//  PolyPortamentoProcessor
//------------------------------------------------------------------------
class PolyPortamentoProcessor : public Steinberg::Vst::AudioEffect
{
public:
	PolyPortamentoProcessor ();
	~PolyPortamentoProcessor () SMTG_OVERRIDE;

    // Create function
	static Steinberg::FUnknown* createInstance (void* /*context*/) 
	{ 
		return (Steinberg::Vst::IAudioProcessor*)new PolyPortamentoProcessor; 
	}

	//--- ---------------------------------------------------------------------
	// AudioEffect overrides:
	//--- ---------------------------------------------------------------------
	/** Called at first after constructor */
	Steinberg::tresult PLUGIN_API initialize (Steinberg::FUnknown* context) SMTG_OVERRIDE;
	
	/** Called at the end before destructor */
	Steinberg::tresult PLUGIN_API terminate () SMTG_OVERRIDE;
	
	/** Switch the Plug-in on/off */
	Steinberg::tresult PLUGIN_API setActive (Steinberg::TBool state) SMTG_OVERRIDE;

	/** Will be called before any process call */
	Steinberg::tresult PLUGIN_API setupProcessing (Steinberg::Vst::ProcessSetup& newSetup) SMTG_OVERRIDE;
	
	/** Asks if a given sample size is supported see SymbolicSampleSizes. */
	Steinberg::tresult PLUGIN_API canProcessSampleSize (Steinberg::int32 symbolicSampleSize) SMTG_OVERRIDE;

	/** Here we go...the process call */
	Steinberg::tresult PLUGIN_API process (Steinberg::Vst::ProcessData& data) SMTG_OVERRIDE;
		
	/** For persistence */
	Steinberg::tresult PLUGIN_API setState (Steinberg::IBStream* state) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API getState (Steinberg::IBStream* state) SMTG_OVERRIDE;

//------------------------------------------------------------------------
protected:
	// suzumushi: 
	// GUI and host facing parameters
	struct GUI_param gp;

	// incoming note on/off message
	struct {
		bool update {false};						// update of note on/off
		int voices {0};								// number of channel voices 
		int32 onSampleOffset {0};					// sample offset of note on message
		int32 offSampleOffset {0};					// sample offset of note off message
		int16 note {0};								// note number
		float velocity {0.0};						// note velocity
	} noteOnOff [16];

	// channel status
	struct {
		bool start {true};							// start of portamento
		int initialPitch {0};
		int currentPitch {0};
		int targetPitch {0};
		int bendCount {0};							// number of transmitted pitch bend messages 
		int numBend {0};							// number of pitch bend messages to be sent
		int clock {0};
	} channelStat [16];

	int interval {800};								// interval of pitch bend messages (samples)
	double tempo {120.0};							// BPM
	NoteToPitch NoteTo;

	// internal functions
	void gui_param_update (const ParamID paramID, const ParamValue paramValue);
	int currentPitch (const int channel) const;
	int numBend (const int channel) const;
	void reset ();	

};

//------------------------------------------------------------------------
} // namespace suzumushi
