#ifndef PLAYER_H
#define PLAYER_H

#include "Includes.h"

class Player
{
private:
	//matriser

	//gdevices f�r memcpy och matris �ndring kanske?

	//man sparar vectorer och matriser som floats pga performance
	DirectX::XMFLOAT3 mDirection; //H�llet man tittar //beh�vs kanske inte
	DirectX::XMFLOAT3 mPosition; //position p� karakt�r
	DirectX::XMFLOAT3 mUp;		//up pekar upp y //XMFLOAT3 dessa �ndras hela tiden
	DirectX::XMFLOAT3 mLook;	//look pekar ner z	//direction
	DirectX::XMFLOAT3 mRight;	//right pekar ner x			//cross produkt av de andra 2
	DirectX::XMFLOAT4X4 mViewMatrix;		//sparad viewmatrix
	DirectX::XMFLOAT4X4 mProjectionMatrix;	//sparad projectionmatris
public:
	Player(); //devices
	virtual ~Player();
	
	void move();
	//update viewMatrix? hmmmmm


};
































#endif