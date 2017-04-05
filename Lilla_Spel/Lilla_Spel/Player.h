#ifndef PLAYER_H
#define PLAYER_H

#include "Includes.h"

class Player
{
private:
	//matriser
	objMatrices matrices;

	//gdevices f�r memcpy och matris �ndring kanske?
	ID3D11Device* gDevice;
	ID3D11DeviceContext* gDeviceContext;

	//man sparar vectorer och matriser som floats pga performance
	DirectX::XMFLOAT3 mPosition; //position p� karakt�r
	//dessa 3: beh�ver inte �ndras f�r ska vara samma?
	DirectX::XMFLOAT3 mUp;		//up pekar upp y //XMFLOAT3 dessa �ndras hela tiden
	DirectX::XMFLOAT3 mForward;	//look pekar ner z	//direction
	DirectX::XMFLOAT3 mRight;	//right pekar ner x			//cross produkt av de andra 2
	

	DirectX::XMFLOAT4X4 cameraWorldMatrix;		//sparad viewmatrix
	//DirectX::XMFLOAT4X4 mProjectionMatrix;	//sparad projectionmatris
public:
	Player(); //devices
	Player(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, DirectX::XMMATRIX cameraWorldMatrix);
	virtual ~Player();
	
	DirectX::XMMATRIX move(DirectX::XMMATRIX worldM); //bara �ndra position vill intr h�lla p� med view matrisen elr n�t

	//matris funktioner som initiate, om matriser skall anv�ndas?

	//D3D11_SUBRESOURCE_DATA getMatricesSubresource();
	//ID3D11Buffer* createConstantBuffer();
	//void updateConstantBuffer(ID3D11Buffer* VSConstantBuffer);
	//ID3D11Buffer* createPlayerPosBuffer();
	//void updatePlayerPosBuffer(ID3D11Buffer* playerConstantBuffer);
	

	void initiateMatrices();
	//update worldMatrix


};
































#endif