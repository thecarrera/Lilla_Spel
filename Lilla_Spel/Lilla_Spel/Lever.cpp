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

void Lever::activateLever()
{
	
	if (m_activatedTimeEnd - m_activatedTime > m_activationDelay ) {
		
		m_activatedTime = GetCurrentTime();


		if (this->leverActivated == false) {
			this ->leverActivated = true;
			cout << "lever activated!" << endl;
		}
		else {
			this->leverActivated = false;
			cout << "lever deactivated!" << endl;
		}
	}

	m_activatedTimeEnd = GetCurrentTime();
}

void Lever::setId(int id)
{
	this->m_id = id;
}

bool Lever::getLeverOnOffState() const
{
	return leverActivated;
}
