#pragma once

#include "Includes.h"
#include "Camera.h"

class Player
{
private:
	objMatrices matrices;

	bool flyingUp;
	DirectX::XMFLOAT3 velocity;

	bool digging;

	time_t currentTime;
	time_t timeWhenBurrowed;
	int burrowCD;
public:
	Player();
	~Player();
	
	void move(Camera* &camera, bool &menuMsg, time_t &tButtonPress, time_t &lTimePress);
	void initiateMatrices(DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj);
	void setMatrices(objMatrices all);
	objMatrices getMatrices()const;

	void flushGame();
};



