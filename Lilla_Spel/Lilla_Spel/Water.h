#pragma once
#include "Includes.h"

class Water
{
public:
	Water();
	~Water();
private:
	float _WaveLength;
	float _WaveHeight;
	float _WaveSpeed;
	float _RandomHeight;
	float _RandomSpeed;
	time_t _Time;


public:
	//Getters

	//Get Wave Length
	float GetWaveLength() {
		return this->_WaveLength;
	};
	//Get Wave Height
	float GetWaveHeight() {
		return this->_WaveHeight;
	}
	//Get Wave Speed
	float GetWaveSpeed() {
		return this->_WaveSpeed;
	}
	//Get the Random Height
	float GetRanHeight() {
		return this->_RandomHeight;
	}
	//Get the Random Speed
	float GetRanSpeed() {
		return this->_RandomSpeed;
	}
	//Get TimeSeed
	float GetTime() {
		_Time = GetCurrentTime();
		return _Time;
	}

	//Setters

	//Set Wave Length
	void SetWaveLength(float wf) {
		this->_WaveLength = wf;
	};
	//Set Wave Height
	void SetWaveHeight(float wh) {
		this->_WaveHeight = wh;
	}
	//Set Wave Speed
	void SetWaveSpeed(float ws) {
		this->_WaveSpeed = ws;
	}
	//Set the Random Height
	void SetRanHeight(float rh) {
		this->_RandomHeight = rh;
	}
	//Set the Random Speed
	void SetRanSpeed(float rs) {
		this->_RandomSpeed = rs;
	}
	
};

Water::Water()
{
}

Water::~Water()
{
}

