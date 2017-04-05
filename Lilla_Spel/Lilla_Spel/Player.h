#ifndef PLAYER_H
#define PLAYER_H

#include "Includes.h"

class Player
{
private:
	//matriser
	struct objMatrices
	{
		DirectX::XMMATRIX worldM;
		DirectX::XMMATRIX viewM;
		DirectX::XMMATRIX projM;
	};
	objMatrices matrices;

	//gdevices för memcpy och matris ändring kanske?
	ID3D11Device* gDevice;
	ID3D11DeviceContext* gDeviceContext;

	//man sparar vectorer och matriser som floats pga performance
	DirectX::XMFLOAT3 mDirection; //Hållet man tittar //behövs kanske inte
	DirectX::XMFLOAT3 mPosition; //position på karaktär
	DirectX::XMFLOAT3 mUp;		//up pekar upp y //XMFLOAT3 dessa ändras hela tiden
	DirectX::XMFLOAT3 mForward;	//look pekar ner z	//direction
	DirectX::XMFLOAT3 mRight;	//right pekar ner x			//cross produkt av de andra 2
	DirectX::XMFLOAT4X4 mViewMatrix;		//sparad viewmatrix
	DirectX::XMFLOAT4X4 mProjectionMatrix;	//sparad projectionmatris
public:
	Player(); //devices
	Player(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext);
	virtual ~Player();
	
	void move();

	D3D11_SUBRESOURCE_DATA getMatricesSubresource();
	ID3D11Buffer* createConstantBuffer();
	ID3D11Buffer* createPlayerPosBuffer();
	void updatePlayerPosBuffer(ID3D11Buffer* playerConstantBuffer);
	//update worldMatrix


};
































#endif