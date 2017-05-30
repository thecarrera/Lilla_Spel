#pragma once

#include "Includes.h"
#include "Camera.h"
using namespace DirectX;
class Player
{
private:
	float movementSpeed = 8;
	float turnSpeed = 4;

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

	void InterpolateTo(float angle, float deltaTime);
public:
	Player();
	~Player();
	
	string move(Camera* &camera, CollisionData* collision, bool &menuMsg, time_t &tButtonPress, time_t &lTimePress, objMatrices& lMatrix, SoundManager& SM, bool canMove, float deltaTime);
	
	void initiateMatrices(DirectX::XMMATRIX& world, DirectX::XMMATRIX& view, DirectX::XMMATRIX& proj);
	
	void setMatrices(objMatrices& all);
	
	objMatrices getMatrices()const;
	
	float rotation = 0;

	bool getIsDigging() const;

	void getPosition(XMFLOAT4& pos);
	float getPositionX();
	float getPositionZ();
	void getPositionVec(XMVECTOR& pos);

	void printMatrices()
	{
		XMFLOAT4X4 w;

		XMStoreFloat4x4(&w, matrices.worldM);

		cout << "World: " << endl;
		cout << w._11 << ", " << w._12 << ", " << w._13 << ", " << w._14 << endl;
		cout << w._21 << ", " << w._22 << ", " << w._23 << ", " << w._24 << endl;
		cout << w._31 << ", " << w._32 << ", " << w._33 << ", " << w._34 << endl;
		cout << w._41 << ", " << w._42 << ", " << w._43 << ", " << w._44 << endl;
	}

	void flushGame();
};



