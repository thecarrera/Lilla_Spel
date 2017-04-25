#include "PressurePlate.h"




PressurePlate::PressurePlate(int activeTime)
{
	this->m_activeTime = activeTime;
	this->m_active = false;
}

PressurePlate::~PressurePlate()
{
}

void PressurePlate::activatePressurePlate()
{
	m_activatedTime = GetCurrentTime();
	m_active = true;
}

PressurePlateData PressurePlate::getPressurePlateData()
{
	PressurePlateData PPD;
	// Check if we are standing on the pp (pressure plate). If the difference between activated time and current
	// time is less than 50ms we are stadning on the pp.
	if (GetCurrentTime() - this->m_activatedTime < 50) {
		cout << "plate is active!" << endl;
		PPD.active = true;
		PPD.ticking = false;
	}
	// If we walk away, the difference will begin to increase and we know we are not on the pp any more.
	// The pp is now in "ticking mode"
	else if (GetCurrentTime() - this->m_activatedTime < m_activeTime) {
		cout << "plate is ticking!" << endl;
		PPD.active = false;
		PPD.ticking = true;
	}
	else {
		//cout << "plate is Not Active" << endl;
		PPD.ticking = false;
		PPD.active = false;
	}


	return PPD;
}
