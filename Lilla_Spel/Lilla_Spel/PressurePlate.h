#pragma once
#include <ctime>
#include <d3d11.h>
#include <iostream>
using namespace std;

struct PressurePlateData
{
	bool active;
	bool ticking;
};

class PressurePlate
{

private:
	bool m_active;
	int m_activeTime;

	time_t m_activatedTime;
public:
	// Custructor. Sets the default active time to 3 seconds (ms) 
	PressurePlate(int activeTime = 3000);
	virtual ~PressurePlate();


	// This functions is called when a pressure plate will be activated. 
	// It will start a timer 
	void activatePressurePlate();

	// Returns the pressure plate data
	PressurePlateData getPressurePlateData();

};