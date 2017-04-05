#include "Player.h"

Player::Player()
{

}

Player::Player(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext)
{
	this->gDevice = gDevice;
	this->gDeviceContext = gDeviceContext;
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

	//jfesnresigreijogrjio fortsätta och fixa karaktär movement
}

D3D11_SUBRESOURCE_DATA Player::getMatricesSubresource()
{
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));

	data.pSysMem = &matrices;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;
	return data;
}

ID3D11Buffer* Player::createConstantBuffer()
{
	D3D11_BUFFER_DESC description;
	ZeroMemory(&description, sizeof(D3D11_BUFFER_DESC));

	description.ByteWidth = sizeof(objMatrices);
	description.Usage = D3D11_USAGE_DYNAMIC;
	description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	description.MiscFlags = 0;
	description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ID3D11Buffer* pBuffer = nullptr;

	D3D11_SUBRESOURCE_DATA matriceResource = getMatricesSubresource();
	HRESULT hr = this->gDevice->CreateBuffer(&description, &matriceResource, &pBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"matrix resource creation failed!", L"error", MB_OK);
	}
	return (pBuffer);
}

ID3D11Buffer* Player::createPlayerPosBuffer()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(DirectX::XMFLOAT4);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	ID3D11Buffer* pBuffer = nullptr;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &this->mPosition;

	HRESULT hr = this->gDevice->CreateBuffer(&desc, &data, &pBuffer);
	if (FAILED(hr))
	{
		MessageBox(0, L"Player buffer cration failed", L"error", MB_OK);
	}

	return pBuffer;
}

void Player::updatePlayerPosBuffer(ID3D11Buffer* playerConstantBuffer)
{
	D3D11_MAPPED_SUBRESOURCE data;
	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));

	this->gDeviceContext->Map(playerConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	memcpy(data.pData, &this->mPosition, sizeof(DirectX::XMFLOAT3));

	this->gDeviceContext->Unmap(playerConstantBuffer, 0);
}
