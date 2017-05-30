#pragma once
#include <iostream>
#include <d3d11.h>
#include "Sound.h"
using namespace std;
class Lever
{

private:
	bool leverActivated;
	int m_activationDelay;
	time_t m_activatedTime;
	time_t m_activatedTimeEnd;
	int m_id;
public:
	Lever();
	virtual ~Lever();

	void activateLever(SoundManager& SM);

	void setId(int id);

	int getId();

	// Gets the state of the lever, if its activated or not
	// Return type bool
	bool getLeverOnOffState() const;
};

