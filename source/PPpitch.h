//
// Copyright (c) 2023 suzumushi
//
// 2023-8-19		PPpitch.h
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 (CC BY-NC-SA 4.0).
//
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//

#pragma once

namespace suzumushi {

constexpr int middleNote {60};				// middle of MIDI note number 
constexpr int sensitivity {48};				// default pitch bend sensitivity of MPE member channels

class NoteToPitch {
public:
	constexpr NoteToPitch ()
	{
		for (int i = 0; i < 128; i++)
			pitchTBL [i] = 0;
		for (int i = 0; i < sensitivity * 2; i++)
			pitchTBL [middleNote - sensitivity + i] = (int)(8192.0 / sensitivity * i + 0.5);
		pitchTBL [middleNote + sensitivity] = 16'383;
	};
	int pitch (const int note) const
	{
		return (pitchTBL [note]);
	};
private:
	int pitchTBL [128];
};

} // suzumushi
