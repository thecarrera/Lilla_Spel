#pragma once
#include <ctime>
#include <d3d11.h>
#include <iostream>
#include "sound.h"
using namespace std;

struct PressurePlateData
{
	bool active;
	bool ticking;
	bool toggled;
};

class PressurePlate
{

private:
	int m_id;
	bool m_active;
	int m_activeTime;

	time_t m_activatedTime;
	bool toggled;

	// time
	int m_activationDelay;
	time_t m_activatedTime2;
	time_t m_activatedTimeEnd;

public:
	// Custructor. Sets the default active time to 3 seconds (ms) 
	PressurePlate(int activeTime = 3000);
	virtual ~PressurePlate();


	// This functions is called when a pressure plate will be activated. 
	// It will start a timer 
	void activatePressurePlate(SoundManager& SM);
	void togglePressurePlate(SoundManager& SM);
	void setActiveTime(int activeTime);
	void setId(int id);

	int getId() const;

	// Returns the pressure plate data
	PressurePlateData getPressurePlateData();

};