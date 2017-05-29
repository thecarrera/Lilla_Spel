#include "Player.h"

Player::Player()
{
	this->flyingUp = false;
	this->velocity = DirectX::XMFLOAT3(0, 0, 0);
	this->digging = false;

	this->currentTime = GetCurrentTime();
	this->burrowCD = 1000;
	this->timeWhenBurrowed = 0;

	this->direction = "nothing";
}
Player::~Player()
{
}

string Player::move(Camera* &camera, CollisionData* collisionData, bool &menuMsg, time_t &tButtonPress, time_t &lTimePress, objMatrices &lMatrix, SoundManager& SM, bool canMove, float deltaTime)
{
	if (GetAsyncKeyState(0x54))
	{
		XMFLOAT4X4 temp;
		XMStoreFloat4x4(&temp, matrices.worldM);

		temp._14 = 790;
		temp._34 = 35;

		matrices.worldM = XMLoadFloat4x4(&temp);

		XMFLOAT3 pos = { 790, 25, 10 };

		camera->setCameraPos(pos);
	
		camera->updateCamera();
	}

	string r = "idle";
	tButtonPress = GetCurrentTime();
	if (GetAsyncKeyState(VK_ESCAPE))//Esc
	{
		if (tButtonPress - lTimePress >= 900)
		{
			lTimePress = GetCurrentTime();
			menuMsg = true;
			SM.pauseAllAmbient(true);
		}
	}

	// If collision is detected, determine which key was last pressed and do an opposite movement action
	if (collisionData[0].collision) {
		
		DirectX::XMFLOAT4X4 CurrentMat, LastMat;
		
		XMMATRIX worldM = matrices.worldM;
		DirectX::XMStoreFloat4x4(&CurrentMat, worldM);
		XMMATRIX lastWorldM = lastWorld;
		DirectX::XMStoreFloat4x4(&LastMat, lastWorldM);

		LastMat._24 = CurrentMat._24;

		lastWorld = DirectX::XMLoadFloat4x4(&LastMat);

		matrices.worldM = lastWorld;
		camera->setView(lastCam);
		return "collision";
	}
	else {
		if (canMove) {
			float movement = movementSpeed * deltaTime;
			if (!((GetAsyncKeyState(0x57) && GetAsyncKeyState(0x53)) || (GetAsyncKeyState(0x41) && GetAsyncKeyState(0x44)))) {
				if (GetAsyncKeyState(0x57)) {
					r = "walk";
					if (GetAsyncKeyState(0x41)) {
						movement *= 0.70710678;
						InterpolateTo(-135, deltaTime);
					}
					else if (GetAsyncKeyState(0x44)) {
						movement *= 0.70710678;
						InterpolateTo(135, deltaTime);
					}
					else {
						InterpolateTo(180, deltaTime);
					}
				}
				else if (GetAsyncKeyState(0x53)) {
					r = "walk";
					if (GetAsyncKeyState(0x41)) {
						movement *= 0.70710678;
						InterpolateTo(-45, deltaTime);
					}
					else if (GetAsyncKeyState(0x44)) {
						movement *= 0.70710678;
						InterpolateTo(45, deltaTime);
					}
					else {
						InterpolateTo(0, deltaTime);
					}
				}
				else {
					if (GetAsyncKeyState(0x41)) {
						r = "walk";
						InterpolateTo(-90, deltaTime);
					}
					else if (GetAsyncKeyState(0x44)) {
						r = "walk";
						InterpolateTo(90, deltaTime);
					}
				}
			}
			else {
				if (!GetAsyncKeyState(0x57) && GetAsyncKeyState(0x53)) {
					r = "walk";
					InterpolateTo(0, deltaTime);
				}
				else if (!GetAsyncKeyState(0x53) && GetAsyncKeyState(0x57)) {
					r = "walk";
					InterpolateTo(180, deltaTime);
				}
				else if (!GetAsyncKeyState(0x41) && GetAsyncKeyState(0x44)) {
					r = "walk";
					InterpolateTo(90, deltaTime);
				}
				else if (!GetAsyncKeyState(0x44) && GetAsyncKeyState(0x41)) {
					r = "walk";
					InterpolateTo(-90, deltaTime);
				}
			}

			lastWorld = matrices.worldM;
			lastCam = camera->getCameraMatrices().viewM;
			// Check which key is pressed and store last key press as int. wsad = 0123
			if (GetAsyncKeyState(0x57)) //w
			{
				lastKeyPressed = 0;
				XMMATRIX worldM = this->matrices.worldM;
				this->matrices.worldM = worldM * DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, movement));
				camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, -movement)));

				XMMATRIX viewM = lMatrix.viewM;
				lMatrix.viewM = viewM * DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, -movement));
				this->direction = "w";
			}

			if (GetAsyncKeyState(0x53))	//s
			{
				lastKeyPressed = 1;
				XMMATRIX worldM = this->matrices.worldM;
				this->matrices.worldM = worldM * DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-0.0f, 0.0f, -movement));
				camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, movement)));
				XMMATRIX viewM = lMatrix.viewM;
				lMatrix.viewM = viewM * DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, movement));
				this->direction = "s";
			}

			if (GetAsyncKeyState(0x41))	//a
			{
				lastKeyPressed = 2;
				XMMATRIX worldM = this->matrices.worldM;
				this->matrices.worldM = worldM * DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-movement, 0.0f, 0.0f));
				camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(movement, 0.0f, 0.0f)));
				XMMATRIX viewM = lMatrix.viewM;
				lMatrix.viewM = viewM * DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(movement, 0.0f, 0.0f));
				this->direction = "a";
			}

			if (GetAsyncKeyState(0x44))	//d
			{
				lastKeyPressed = 3;
				XMMATRIX worldM = this->matrices.worldM;
				this->matrices.worldM = worldM * DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(movement, 0.0f, 0.0f));
				camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-movement, 0.0f, 0.0f)));
				XMMATRIX viewM = lMatrix.viewM;
				lMatrix.viewM = viewM * DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-movement, 0.0f, -0.0f));
				this->direction = "d";
			}

		}
	}
	
	if (GetAsyncKeyState(0x51)) //q
	{
		if (this->flyingUp == false)
		{
			if ((this->timeWhenBurrowed + burrowCD) <= this->currentTime)
			{
				this->timeWhenBurrowed = GetCurrentTime();

				if (this->digging == true && collisionData[0].collisionType != 1)
				{
					r = "emerge";
					this->digging = false;
					if (tButtonPress - lTimePress >= 200)
					{
						SM.playSound(6);
						SM.setVolume(6, 1.4f);
						lTimePress = GetCurrentTime();
					}
					//camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, -5.0f, 0.0f)));
					//XMMATRIX worldM = this->matrices.worldM;
					//this->matrices.worldM = worldM * DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 10.0f, 0.0f));
				}
				else if(collisionData[0].collisionType != 1)
				{
					r = "dive";
					this->digging = true;
					if (tButtonPress - lTimePress >= 200)
					{
						SM.playSound(6);
						SM.setVolume(6, 1.4f);
						lTimePress = GetCurrentTime();
					}
					//if (collisionData[0].collisionType != 1) {
						//camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 5.0f, 0.0f)));
						// WAS OUTSIDE IF STATEMENT BEFORE, PRESUMED MISTAKE AND MOVED INSIDE
						//XMMATRIX worldM = this->matrices.worldM;
						//this->matrices.worldM = worldM * DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, -10.0f, 0.0f));
					//}
				}
			}
		}
	}

	if (GetAsyncKeyState(VK_SPACE))	//space
	{
		if (digging == false)
		{
			if (this->flyingUp == false)
			{
				//this->velocity.y = -1.0f;
				r = "jump";
				this->flyingUp = true;
			}
		}
	}

	if (this->flyingUp == true)
	{
		this->flyingUp = false;
		//this->velocity.y += 0.1f;

		//if (velocity.y >= 0.9f)		//Sphagetti code ftw
		//{
		//	flyingUp = false;
		//}
		//XMMATRIX worldM = this->matrices.worldM;
		//this->matrices.worldM = worldM * DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(velocity.x, -velocity.y, velocity.z));
		if (tButtonPress - lTimePress >= 620)
		{
			SM.playSound(5);
			SM.setVolume(5, 1.4f);
			lTimePress = GetCurrentTime();
		}
		// if payer hitbox = hitbox ground -> velocity.y = 0;
	}
	this->currentTime = GetCurrentTime();

	DirectX::XMFLOAT4X4 tempWorldMat;
	FMOD_VECTOR tempPlayerPos;
	XMMATRIX worldM = this->matrices.worldM;
	DirectX::XMStoreFloat4x4(&tempWorldMat, worldM);
	tempPlayerPos.x = tempWorldMat._14;
	tempPlayerPos.y = tempWorldMat._24;
	tempPlayerPos.y = tempWorldMat._34;

	SM.setListnerPos(tempPlayerPos);
	return r;
}

void Player::initiateMatrices(DirectX::XMMATRIX& world, DirectX::XMMATRIX& view, DirectX::XMMATRIX& proj)
{
	this->matrices.worldM = world;
	this->matrices.viewM = view;
	this->matrices.projM = proj;
}
void Player::setMatrices(objMatrices& all)
{
	this->matrices.worldM = all.worldM;
	this->matrices.viewM = all.viewM;
	this->matrices.projM = all.projM;
}
objMatrices Player::getMatrices()const
{
	return this->matrices;
}

void Player::flushGame()
{
	this->flyingUp = false;
	this->velocity.x = 0;
	this->velocity.y = 0;
	this->velocity.z = 0;
	this->digging = false;
	this->burrowCD = 3000;
	this->timeWhenBurrowed = 0;
}


bool Player::getIsDigging() const
{
	return digging;
}

void Player::getPosition(XMFLOAT4 & pos)
{
	XMFLOAT4X4 temp;

	XMMATRIX worldM = matrices.worldM;
	XMStoreFloat4x4(&temp, worldM);

	pos.x = temp._14;
	pos.z = temp._34;
	
}

float Player::getPositionX()
{
	XMFLOAT4X4 temp;

	XMStoreFloat4x4(&temp, matrices.worldM);

	return temp._14;
}

void Player::getPositionVec(XMVECTOR & pos)
{
	XMFLOAT4 test;
	test.y = 0;
	getPosition(test);


	pos = XMLoadFloat4(&test);

}

XMFLOAT3 Player::getPlayerPos()
{
	DirectX::XMFLOAT4X4 temp;

	DirectX::XMStoreFloat4x4(&temp, this->matrices.worldM);

	DirectX::XMFLOAT3 playerPos = { 0, 0, 0 };
	playerPos.x = temp._14;
	playerPos.y = temp._24;
	playerPos.z = temp._34;

	return playerPos;
}

void Player::InterpolateTo(float angle, float deltaTime) {
	bool rightOrder = true;
	float difference = 0;
	if (angle != rotation) {
		float rightOrderDiff;
		if (angle > rotation) {
			rightOrderDiff = angle - rotation;
		}
		else {
			rightOrderDiff = angle - rotation + 360;
		}
		if (angle < rotation) {
			if (rightOrderDiff > rotation - angle) {
				rightOrder = false;
				difference = rotation - angle;
			}
			else {
				difference = rightOrderDiff;
			}
		}
		else {
			if (rightOrderDiff > rotation - angle + 360) {
				rightOrder = false;
				difference = rotation - angle + 360;
			}
			else {
				difference = rightOrderDiff;
			}
		}
	}

	if (!rightOrder) {
		difference = -difference;
	}

	rotation += difference * turnSpeed * deltaTime;

	if (rotation < -180) {
		rotation += 360;
	}
	else if (rotation > 180) {
		rotation -= 360;
	}
}
bool Player::getFlyingUp()
{
	return this->flyingUp;
}
string Player::getDirection()
{
	return this->direction;
}