#include "Player.h"

Player::Player()
{

}

Player::~Player()
{

}

void Player::move()
{
	if (GetAsyncKeyState(VK_ESCAPE))//Esc
	{
		PostQuitMessage(0);
	}

	if (GetAsyncKeyState(0x57)) //w
	{
		this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.2f));
	}

	if (GetAsyncKeyState(0x53))	//s
	{
		this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.0f, -0.2f));
	}

	if (GetAsyncKeyState(0x41))	//a
	{
		this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(-0.1f, 0.0f, 0.0f));
	}

	if (GetAsyncKeyState(0x44))	//d
	{
		this->matrices.worldM *= DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.1f, 0.0f, 0.0f));
	}

	if (GetAsyncKeyState(VK_SPACE))	//space
	{
		this->matrices.worldM = this->matrices.worldM * DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(0.0f, 0.1f, 0.0f));
	}
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