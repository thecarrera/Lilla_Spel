#pragma once

#include "Includes.h"
#include "Camera.h"
using namespace DirectX;
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
	~Player();
	
	void move(Camera* &camera, CollisionData* collision, bool &menuMsg, time_t &tButtonPress, time_t &lTimePress, objMatrices& lMatrix);
	void initiateMatrices(DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj);
	void setMatrices(objMatrices all);
	objMatrices getMatrices()const;

	bool getIsDigging() const;

	void getPosition(XMFLOAT4& pos);

	void getPositionVec(XMVECTOR& pos);

	void flushGame();
};



