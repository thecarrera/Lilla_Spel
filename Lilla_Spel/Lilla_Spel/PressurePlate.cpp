#include "PressurePlate.h"




PressurePlate::PressurePlate(int activeTime)
{
	this->m_activeTime = activeTime;
	this->m_active = false;
	this->toggled = false;

	m_activatedTime = 0;
	m_activatedTimeEnd = 1;
	m_activationDelay = 800;
}

PressurePlate::~PressurePlate()
{
}

void PressurePlate::activatePressurePlate(SoundManager& SM)
{
	m_activatedTime = GetCurrentTime();
	m_active = true;

	this->soundTick1 = GetCurrentTime();
	if (this->soundTick1 - this->soundTick2 >= 1500) 
	{
		SM.playSound(10);
		SM.setVolume(10, 1.7f);

		this->soundTick2 = GetCurrentTime();
	}

}



void PressurePlate::setActiveTime(int activeTime)
{
	this->m_activeTime = activeTime;
}

void PressurePlate::setId(int id)
{
	this->m_id = id;
}

int PressurePlate::getId() const
{
	return m_id;
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
	else if (toggled)
	{
		PPD.toggled = true;
	}
	else {
		//cout << "plate is Not Active" << endl;
		PPD.ticking = false;
		PPD.active = false;
		PPD.toggled = false;
	}


	return PPD;
}
void PressurePlate::togglePressurePlate(SoundManager& SM)
{
	if (m_activatedTimeEnd - m_activatedTime2 > m_activationDelay) {

		m_activatedTime2 = GetCurrentTime();
		SM.playSound(10);
		SM.setVolume(10, 1.7f);
		
		if (this->toggled == false) {
			this->toggled = true;
			cout << "Pressureplate toggled on!" << endl;
		}
		else {
			this->toggled = false;
			cout << "Pressureplate toggled off!" << endl;
			SM.soundChannel[16]->setPitch(0.7f);
		}
	}

	m_activatedTimeEnd = GetCurrentTime();

}