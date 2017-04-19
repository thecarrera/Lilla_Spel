#ifndef PLAYER_H
#define PLAYER_H

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
	virtual ~Player();
	
	void move(Camera* &camera);
	void initiateMatrices(DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj);
	void setMatrices(objMatrices all);
	objMatrices getMatrices()const;
};
































#endif