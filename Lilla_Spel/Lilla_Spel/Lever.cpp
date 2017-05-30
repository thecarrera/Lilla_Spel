#include "Lever.h"



Lever::Lever()
{
	this->leverActivated = false;
	m_activatedTime = 0;
	m_activatedTimeEnd = 1;
	m_activationDelay = 800;
}


Lever::~Lever()
{
}

void Lever::activateLever(SoundManager& SM)
{
	
	if (m_activatedTimeEnd - m_activatedTime > m_activationDelay ) 
	{
		m_activatedTime = GetCurrentTime();

		SM.playSound(9);
		SM.setVolume(9, 1.6f);
	
		if (this->leverActivated == false) {
			this ->leverActivated = true;
			cout << "lever activated!" << endl;
		}
		else {
			this->leverActivated = false;
			cout << "lever deactivated!" << endl;
			SM.soundChannel[15]->setPitch(0.7f);
		}
	}

	m_activatedTimeEnd = GetCurrentTime();
}

void Lever::setId(int id)
{
	this->m_id = id;
}

int Lever::getId()
{
	return this->m_id;
}

bool Lever::getLeverOnOffState() const
{
	return leverActivated;
}
