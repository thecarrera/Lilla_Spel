#ifndef PLAYER_H
#define PLAYER_H

#include "Includes.h"
#include "Camera.h"

class Player
{
private:
	objMatrices matrices;

	XMMATRIX lastWorld;
	XMMATRIX lastCam;

	bool flyingUp;
	DirectX::XMFLOAT3 velocity;

	bool digging;
	bool camDown;

	time_t currentTime;
	time_t timeWhenBurrowed;
	int burrowCD;

	int lastKeyPressed = -1;
	bool canPressW = true;
	bool canPressS = true;
	bool canPressA = true;
	bool canPressD = true;
public:
	Player();
	virtual ~Player();
	
	void move(Camera* &camera, CollisionData* collision);
	void initiateMatrices(DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj);
	void setMatrices(objMatrices all);
	objMatrices getMatrices()const;

	bool getIsDigging() const;
};
































#endif