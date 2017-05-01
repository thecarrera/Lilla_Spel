#include "Player.h"

Player::Player()
{
	this->flyingUp = false;
	this->velocity = DirectX::XMFLOAT3(0, 0, 0);
	this->digging = false;

	this->currentTime = GetCurrentTime();
	this->burrowCD = 3000;
	this->timeWhenBurrowed = 0;
}

Player::~Player()
{

}

void Player::move(Camera* &camera, CollisionData* collisionData )
{
	if (GetAsyncKeyState(VK_ESCAPE))//Esc
	{
		PostQuitMessage(0);
	}
	

	// If collision is detected, determine which key was last pressed and do an opposite movement action
	if (collisionData[0].collision) {
		switch (lastKeyPressed)
		{
		case 0:
			this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, -0.2f));
			camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.2f)));
			break;
		case 1:
			this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.2f));
			camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, -0.2f)));
			break;
		case 2:
			this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.2f, -0.0f, 0.0f));
			camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-0.2f, 0.0f, 0.0f)));
			break;
		case 3:
			this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-0.2f, -0.0f, 0.0f));
			camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.2f, 0.0f, 0.0f)));
			break;
		}
		return;
	}

	// Check which key is pressed and store last key press as int. wsad = 0123
	if (GetAsyncKeyState(0x57)) //w
	{
		lastKeyPressed = 0;
		this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.2f));
		camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, -0.2f)));
	}

	if (GetAsyncKeyState(0x53))	//s
	{
		lastKeyPressed = 1;
		this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-0.0f, 0.0f, -0.2f));	
		camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.2f)));
	}

	if (GetAsyncKeyState(0x41))	//a
	{
		lastKeyPressed = 2;
		this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-0.2f, 0.0f, 0.0f));
		camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.2f, 0.0f, 0.0f)));
  }

	if (GetAsyncKeyState(0x44))	//d
	{
		lastKeyPressed = 3;
		this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.2f, 0.0f, 0.0f));
		camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-0.2f, 0.0f, 0.0f)));
	}

	
	if (GetAsyncKeyState(0x51)) //q
	{
		if (this->flyingUp == false)
		{
			if ((this->timeWhenBurrowed + burrowCD) <= this->currentTime)
			{
				this->timeWhenBurrowed = GetCurrentTime();

				if (this->digging == true)
				{
					this->digging = false;
				}
				else
				{
					this->digging = true;
				}

				if (this->digging == true)
				{
					camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 5.0f, 0.0f)));		
				}

				if (this->digging == false)
				{
					camera->move(DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, -5.0f, 0.0f)));
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

bool Player::getIsDigging() const
{
	return digging;
}
