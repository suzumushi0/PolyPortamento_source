//
// Copyright (c) 2023 suzumushi
//
// 2023-10-11		PPparam.h
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 (CC BY-NC-SA 4.0).
//
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//

#pragma once

#include "SOextparam.h"


namespace suzumushi {

// definitions of GUI and host facing parameter tag

constexpr ParamID P_TIME {0};			// portamento time
constexpr ParamID TIME_UNIT {1};		// units of portamento time
constexpr ParamID P_CURVE {2};			// portmento curve

// definitions of MIDI CC and message parameter tag

constexpr ParamID PITCH_BEND_01 {100};	// channel 1 pitch bend
constexpr ParamID PITCH_BEND_02 {101};	// channel 2 pitch bend
constexpr ParamID PITCH_BEND_03 {102};	// channel 3 pitch bend
constexpr ParamID PITCH_BEND_04 {103};	// channel 4 pitch bend
constexpr ParamID PITCH_BEND_05 {104};	// chaneel 5 pitch bend
constexpr ParamID PITCH_BEND_06 {105};	// channel 6 pitch bend
constexpr ParamID PITCH_BEND_07 {106};	// channel 7 pitch bend
constexpr ParamID PITCH_BEND_08 {107};	// channel 8 pitch bend
constexpr ParamID PITCH_BEND_09 {108};	// channel 9 pitch bend
constexpr ParamID PITCH_BEND_10 {109};	// chaneel 10 pitch bend
constexpr ParamID PITCH_BEND_11 {110};	// chaneel 11 pitch bend
constexpr ParamID PITCH_BEND_12 {111};	// chaneel 12 pitch bend
constexpr ParamID PITCH_BEND_13 {112};	// chaneel 13 pitch bend
constexpr ParamID PITCH_BEND_14 {113};	// chaneel 14 pitch bend
constexpr ParamID PITCH_BEND_15 {114};	// chaneel 15 pitch bend
constexpr ParamID PITCH_BEND_16 {115};	// chaneel 16 pitch bend

// attributes of GUI and host facing parameter

constexpr struct logTaperParameter p_time = {
	P_TIME,								// tag
	{0.0},								// min
	{8.0},								// max
	{0.0},								// default
	{0},								// continuous
	{ParameterInfo::kCanAutomate}		// flags
};

constexpr struct stringListParameter time_unit = {
	TIME_UNIT,							// tag
	{ParameterInfo::kIsList | ParameterInfo::kCanAutomate}	// flags
};
enum class TIME_UNIT_L {
	SEC,
	SECOCT,
	BEAT,
	BEATOCT,
	LIST_LEN
};

constexpr struct rangeParameter p_curve = {
	P_CURVE,							// tag
	{-3.0},								// min
	{3.0},								// max
	{0.0},								// default
	{0},								// continuous
	{ParameterInfo::kCanAutomate}		// flags
};

constexpr struct rangeParameter pitch_bend = {
	PITCH_BEND_01,						// tag
	{-48.0},							// min
	{48.0},								// max
	{0.0},								// default
	{0},								// continuous
	{ParameterInfo::kCanAutomate}		// flags
};

//  GUI and host facing parameters in processor class

struct GUI_param {
	ParamValue p_time;
	int32 time_unit;
	ParamValue p_curve;
	GUI_param () {
		p_time = suzumushi::p_time.def;
		time_unit = (int32) TIME_UNIT_L::SEC;
		p_curve = suzumushi::p_curve.def;
	}
};

} // namespace suzumushi

