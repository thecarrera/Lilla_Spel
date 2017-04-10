#ifndef PLAYER_H
#define PLAYER_H

#include "Includes.h"

class Player
{
private:
	objMatrices matrices;
public:
	Player();
	virtual ~Player();
	
	void move();
	void initiateMatrices(DirectX::XMMATRIX world, DirectX::XMMATRIX view, DirectX::XMMATRIX proj);
	void setMatrices(objMatrices all);
	objMatrices getMatrices()const;
};
































#endif