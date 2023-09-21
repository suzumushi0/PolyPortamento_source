//
// Copyright (c) 2023 suzumushi
//
// 2023-9-10		PPparam.h
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

// attributes of GUI and host facing parameter

constexpr struct logTaperParameter p_time = {
	P_TIME,								// tag
	{0.0},								// min
	{4.0},								// max
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

