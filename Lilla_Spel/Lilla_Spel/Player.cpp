#include "Player.h"

Player::Player()
{

}

Player::Player(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, DirectX::XMMATRIX cameraWorldMatrix)
{
	DirectX::XMStoreFloat4x4(&this->cameraWorldMatrix, cameraWorldMatrix);

	this->mPosition = DirectX::XMFLOAT3(0, 0, 0);

	// These 3 does not need to be updated I think, so we just define them here
	this->mUp = DirectX::XMFLOAT3(0, 1, 0);
	this->mForward = DirectX::XMFLOAT3(0, 0, 1);
	this->mRight = DirectX::XMFLOAT3(1, 0, 0);
	

	this->gDevice = gDevice;
	this->gDeviceContext = gDeviceContext;

	this->initiateMatrices();
}

Player::~Player()
{

}

DirectX::XMMATRIX Player::move(DirectX::XMMATRIX worldM)
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
		//std::string message = "Up key press";
		//MessageBox(NULL, message.c_str(), NULL, NULL);

		keyboardAmount.z = 1;

		//test, uppdaterar world?

		DirectX::XMMATRIX temp;
		temp = 
		  { 0.02f,0,0,0,
			0,0.02f,0,0,
			0,0,0.02f,0,
			0,0,-10,1 };
		temp = DirectX::XMMatrixTranspose(temp);
		worldM += temp;
	}

	if (GetAsyncKeyState(0x53))	//s
	{
		DirectX::XMMATRIX temp;
		temp =
		{ 0.02f,0,0,0,
			0,0.02f,0,0,
			0,0,0.02f,0,
			0,0,10,1 };
		temp = DirectX::XMMatrixTranspose(temp);
		worldM += temp; 

		keyboardAmount.z = -1;
	}

	if (GetAsyncKeyState(0x41))	//a
	{
		DirectX::XMMATRIX temp;
		temp =
		{ 0.02f,0,0,0,
			0,0.02f,0,0,
			0,0,0.02f,0,
			-10,0,0,1 };
		temp = DirectX::XMMatrixTranspose(temp);
		worldM += temp;

		keyboardAmount.x = -1;
	}

	if (GetAsyncKeyState(0x44))	//d
	{
		DirectX::XMMATRIX temp;
		temp =
		{ 0.02f,0,0,0,
			0,0.02f,0,0,
			0,0,0.02f,0,
			10,0,0,1 };
		temp = DirectX::XMMatrixTranspose(temp);
		worldM += temp;

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
	return worldM;
}

//D3D11_SUBRESOURCE_DATA Player::getMatricesSubresource()
//{
//	D3D11_SUBRESOURCE_DATA data;
//	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
//
//	data.pSysMem = &matrices;
//	data.SysMemPitch = 0;
//	data.SysMemSlicePitch = 0;
//	return data;
//}

//ID3D11Buffer* Player::createConstantBuffer()
//{
//	D3D11_BUFFER_DESC description;
//	ZeroMemory(&description, sizeof(D3D11_BUFFER_DESC));
//
//	description.ByteWidth = sizeof(objMatrices);
//	description.Usage = D3D11_USAGE_DYNAMIC;
//	description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	description.MiscFlags = 0;
//	description.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//
//	ID3D11Buffer* pBuffer = nullptr;
//
//	D3D11_SUBRESOURCE_DATA matriceResource = getMatricesSubresource();
//	HRESULT hr = this->gDevice->CreateBuffer(&description, &matriceResource, &pBuffer);
//	if (FAILED(hr))
//	{
//		MessageBox(0, "matrix resource creation failed!", "error", MB_OK);
//	}
//	return (pBuffer);
//}

//void Player::updateConstantBuffer(ID3D11Buffer* VSConstantBuffer)
//{
//	D3D11_MAPPED_SUBRESOURCE dataPtr;
//	ZeroMemory(&dataPtr, sizeof(D3D11_MAPPED_SUBRESOURCE));
//
//	//Låser buffern för GPU:n och hämtar den till CPU
//	this->gDeviceContext->Map(VSConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
//
//	DirectX::XMMATRIX translation;
//	translation = DirectX::XMMatrixTranslation(0.05f, 0.0f, 0.0f);
//	//translation = DirectX::XMMatrixTranspose(translation);
//	matrices.worldM *= translation;
//
//	//beror på, ska vi transposa en world matris istället?
//
//	memcpy(dataPtr.pData, &matrices, sizeof(matrices));
//
//	//Ger GPU:n tillgång till datan igen
//	this->gDeviceContext->Unmap(VSConstantBuffer, 0);
//}



//ID3D11Buffer* Player::createPlayerPosBuffer()	//kan användas för typ ljus och grejer, borde nog använda kamerans position istället tho
//{
//	D3D11_BUFFER_DESC desc;
//	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
//	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	desc.ByteWidth = sizeof(DirectX::XMFLOAT4);
//	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	desc.Usage = D3D11_USAGE_DYNAMIC;
//
//	ID3D11Buffer* pBuffer = nullptr;
//	D3D11_SUBRESOURCE_DATA data;
//	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
//	data.pSysMem = &this->mPosition;
//
//	HRESULT hr = this->gDevice->CreateBuffer(&desc, &data, &pBuffer);
//	if (FAILED(hr))
//	{
//		MessageBox(0, "Player buffer cration failed", "error", MB_OK);
//	}
//
//	return pBuffer;
//}

//void Player::updatePlayerPosBuffer(ID3D11Buffer* playerConstantBuffer)
//{
//	D3D11_MAPPED_SUBRESOURCE data;
//	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));
//
//	this->gDeviceContext->Map(playerConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
//	memcpy(data.pData, &this->mPosition, sizeof(DirectX::XMFLOAT3));
//
//	this->gDeviceContext->Unmap(playerConstantBuffer, 0);
//}

void Player::initiateMatrices()
{
	DirectX::XMMATRIX worldM =
	{ 1.0f,0,0,0,
		0,1.0f,0,0,
		0,0,1.0f,0,
		0,0,0,1 };

	DirectX::XMMATRIX viewM =
	{ 1.0f,0,0,0,
		0,1.0f,0,0,
		0,0,1.0f,0,
		0,0,0,1 };

	DirectX::XMMATRIX projM =
	{ 1.0f,0,0,0,
		0,1.0f,0,0,
		0,0,1.0f,0,
		0,0,0,1 };

	this->matrices = { DirectX::XMMatrixTranspose(worldM), DirectX::XMMatrixTranspose(viewM), DirectX::XMMatrixTranspose(projM) };

}
