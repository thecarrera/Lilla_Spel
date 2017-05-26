#include "Player.h"

Player::Player()
{
	this->flyingUp = false;
	this->velocity = DirectX::XMFLOAT3(0, 0, 0);
	this->digging = false;

	this->currentTime = GetCurrentTime();
	this->burrowCD = 1000;
	this->timeWhenBurrowed = 0;
}
Player::~Player()
{
}

void Player::move(Camera* &camera, CollisionData* collisionData, bool &menuMsg, time_t &tButtonPress, time_t &lTimePress, objMatrices &lMatrix)
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



	if (GetAsyncKeyState(VK_ESCAPE))//Esc
	{
		tButtonPress = GetCurrentTime();
		if (tButtonPress - lTimePress >= 900)
		{
			lTimePress = GetCurrentTime();
			menuMsg = true;
		}
	}

	// If collision is detected, determine which key was last pressed and do an opposite movement action
	if (collisionData[0].collision) {
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
			
			this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.2f));

			camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, -0.2f)));

			lMatrix.viewM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, -0.2f));
		}

		if (GetAsyncKeyState(0x53))	//s
		{
			
			this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-0.0f, 0.0f, -0.2f));	
			camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.2f)));
			lMatrix.viewM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.2f));
		}

		if (GetAsyncKeyState(0x41))	//a
		{
			
			this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-0.2f, 0.0f, 0.0f));
			camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.2f, 0.0f, 0.0f)));
			lMatrix.viewM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.2f, 0.0f, 0.0f));
	  }

		if (GetAsyncKeyState(0x44))	//d
		{
			
			this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.2f, 0.0f, 0.0f));
			camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-0.2f, 0.0f, 0.0f)));
			lMatrix.viewM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-0.2f, 0.0f, -0.0f));
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
					camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, -5.0f, 0.0f)));
				}
				else
				{
					this->digging = true;
					if (collisionData[0].collisionType != 1)
						camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 5.0f, 0.0f)));
				}
			}
		}
	}

	if (GetAsyncKeyState(VK_SPACE))	//space
	{
		if (this->flyingUp == false)
		{
			this->velocity.y = -1.5f;
			this->flyingUp = true;
		}
	}

	if (this->flyingUp == true)
	{
		this->velocity.y += 0.1f;

		if (velocity.y >= 1.4f)		//Sphagetti code ftw
		{
			flyingUp = false;
		}

		camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(velocity.x, velocity.y, velocity.z)));

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
