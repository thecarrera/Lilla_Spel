#pragma once
#include <iostream>
#include <d3d11.h>
using namespace std;
class Lever
{

private:
	bool leverActivated;
	int m_activationDelay;
	time_t m_activatedTime;
	time_t m_activatedTimeEnd;
public:
	Lever();
	virtual ~Lever();

	void activateLever();
};

