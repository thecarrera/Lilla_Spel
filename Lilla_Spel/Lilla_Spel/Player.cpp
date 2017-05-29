#include "Player.h"

Player::Player()
{
	this->flyingUp = false;
	this->velocity = DirectX::XMFLOAT3(0, 0, 0);
	this->digging = false;

	this->currentTime = GetCurrentTime();
	this->burrowCD = 3000;
	this->timeWhenBurrowed = 0;

	this->direction = "nothing";
}
Player::~Player()
{
}

void Player::move(Camera* &camera, CollisionData* collisionData, bool &menuMsg, time_t &tButtonPress, time_t &lTimePress, objMatrices &lMatrix, SoundManager& SM)
{
	tButtonPress = GetCurrentTime();
	if (GetAsyncKeyState(VK_ESCAPE))//Esc
	{
		if (tButtonPress - lTimePress >= 900)
		{
			lTimePress = GetCurrentTime();
			menuMsg = true;
			SM.togglePauseSound(0, true);
			SM.togglePauseSound(1, true);
			SM.togglePauseSound(2, true);
		}
	}

	// If collision is detected, determine which key was last pressed and do an opposite movement action
	if (collisionData[0].collision) {
		
		DirectX::XMFLOAT4X4 CurrentMat, LastMat;
		
		DirectX::XMStoreFloat4x4(&CurrentMat, matrices.worldM);
		DirectX::XMStoreFloat4x4(&LastMat, lastWorld);

		LastMat._24 = CurrentMat._24;

		lastWorld = DirectX::XMLoadFloat4x4(&LastMat);

		matrices.worldM = lastWorld;
		camera->setView(lastCam);
		return;
	}
	else {
		lastWorld = matrices.worldM;
		lastCam = camera->getCameraMatrices().viewM;
		// Check which key is pressed and store last key press as int. wsad = 0123
		if (GetAsyncKeyState(0x57)) //w
		{
			lastKeyPressed = 0;
			this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.2f));
			camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, -0.2f)));

			lMatrix.viewM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, -0.2f));
			this->direction = "w";
		}

		if (GetAsyncKeyState(0x53))	//s
		{
			lastKeyPressed = 1;
			this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-0.0f, 0.0f, -0.2f));	
			camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.2f)));
			lMatrix.viewM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.2f));
			this->direction = "s";
		}

		if (GetAsyncKeyState(0x41))	//a
		{
			lastKeyPressed = 2;
			this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-0.2f, 0.0f, 0.0f));
			camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.2f, 0.0f, 0.0f)));
			lMatrix.viewM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.2f, 0.0f, 0.0f));
			this->direction = "a";
		}

		if (GetAsyncKeyState(0x44))	//d
		{
			lastKeyPressed = 3;
			this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.2f, 0.0f, 0.0f));
			camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-0.2f, 0.0f, 0.0f)));
			lMatrix.viewM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-0.2f, 0.0f, -0.0f));
			this->direction = "d";
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
					this->digging = false;
					if (tButtonPress - lTimePress >= 200)
					{
						SM.playSound(2);
						SM.setVolume(2, 1.4f);
						lTimePress = GetCurrentTime();
					}
					camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, -5.0f, 0.0f)));
					this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 10.0f, 0.0f));
				}
				else
				{
					this->digging = true;
					if (tButtonPress - lTimePress >= 200)
					{
						SM.playSound(2);
						SM.setVolume(2, 1.4f);
						lTimePress = GetCurrentTime();
					}
					if (collisionData[0].collisionType != 1)
						camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 5.0f, 0.0f)));
						this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, -10.0f, 0.0f));
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
				this->velocity.y = -1.0f;
				this->flyingUp = true;
			}
		}
	}

	if (this->flyingUp == true)
	{
		this->velocity.y += 0.1f;

		if (velocity.y >= 0.9f)		//Sphagetti code ftw
		{
			flyingUp = false;
		}
		this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(velocity.x, -velocity.y, velocity.z));
		if (tButtonPress - lTimePress >= 200)
		{
			SM.playSound(1);
			SM.soundChannel[1]->setVolume(1.4f);
			lTimePress = GetCurrentTime();
		}
		// if payer hitbox = hitbox ground -> velocity.y = 0;
	}
	this->currentTime = GetCurrentTime();
}

void Player::initiateMatrices(DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj)
{
	this->matrices.worldM = world;
	this->matrices.viewM = view;
	this->matrices.projM = proj;
}
void Player::setMatrices(objMatrices all)
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
	this->direction = "nothing";
}

bool Player::getIsDigging() const
{
	return digging;
}

void Player::getPosition(XMFLOAT4 & pos)
{
	XMFLOAT4X4 temp;

	XMStoreFloat4x4(&temp, matrices.worldM);

	pos.x = temp._14;
	pos.z = temp._34;
	
}

void Player::getPositionVec(XMVECTOR & pos)
{
	XMFLOAT4 test;
	test.y = 0;
	getPosition(test);


	pos = XMLoadFloat4(&test);

}

DirectX::XMFLOAT3 Player::getPlayerPos()
{
	DirectX::XMFLOAT4X4 temp;

	DirectX::XMStoreFloat4x4(&temp, this->matrices.worldM);

	DirectX::XMFLOAT3 playerPos = { 0, 0, 0 };
	playerPos.x = temp._14;
	playerPos.y = temp._24;
	playerPos.z = temp._34;
	
	return playerPos;
}

DirectX::XMFLOAT3 Player::getVelocity()
{
	return this->velocity;
}

bool Player::getFlyingUp()
{
	return this->flyingUp;
}

string Player::getDirection()
{
	return this->direction;
}