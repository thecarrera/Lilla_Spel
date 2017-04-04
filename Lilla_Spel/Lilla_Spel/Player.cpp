#include "Player.h"

Player::Player()
{

}

Player::~Player()
{

}

void Player::move()
{
	float defaultMovementRate = 0.02f;

	DirectX::XMFLOAT3 movement;
	DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&this->mPosition); //Positionen på karaktären
	DirectX::XMFLOAT3 keyboardAmount = DirectX::XMFLOAT3(0, 0, 0);

	if (GetAsyncKeyState(VK_ESCAPE))//Esc
	{
		PostQuitMessage(0);
	}

	if (GetAsyncKeyState(0x57)) //w
	{
		std::string message = "Up key press";
		MessageBox(NULL, message.c_str(), NULL, NULL);

		keyboardAmount.z = 1;
	}

	if (GetAsyncKeyState(0x53))	//s
	{
		std::string message = "Down key press";
		MessageBox(NULL, message.c_str(), NULL, NULL);

		keyboardAmount.z = -1;
	}

	if (GetAsyncKeyState(0x41))	//a
	{
		std::string message = "Left key press";
		MessageBox(NULL, message.c_str(), NULL, NULL);

		keyboardAmount.x = -1;
	}

	if (GetAsyncKeyState(0x44))	//d
	{
		std::string message = "Right key press";
		MessageBox(NULL, message.c_str(), NULL, NULL);

		keyboardAmount.x = 1;
	}

	if (GetAsyncKeyState(VK_SPACE))	//space
	{
		std::string message = "Space key press";
		MessageBox(NULL, message.c_str(), NULL, NULL);

		//jump, y led, senare
	}

	movement.x = (keyboardAmount.x * defaultMovementRate);
	movement.y = 0;
	movement.z = (keyboardAmount.z * defaultMovementRate);

	DirectX::XMFLOAT3 floatStrafe; //sides
	floatStrafe.x = this->mRight.x * movement.x;
	floatStrafe.y = this->mRight.y * movement.x;
	floatStrafe.z = this->mRight.z * movement.x;

	DirectX::XMVECTOR strafe = DirectX::XMLoadFloat3(&floatStrafe);

	position = DirectX::XMVectorSetX(position, DirectX::XMVectorGetX(position) + DirectX::XMVectorGetX(strafe)); //bara denna behövs? osäker.
	position = DirectX::XMVectorSetY(position, DirectX::XMVectorGetY(position) + DirectX::XMVectorGetY(strafe));
	position = DirectX::XMVectorSetZ(position, DirectX::XMVectorGetZ(position) + DirectX::XMVectorGetZ(strafe));

	jfesnresigreijogrjio fortsätta och fixa karaktär movement
}