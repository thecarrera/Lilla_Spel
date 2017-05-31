#pragma once

#include "Includes.h"

class Camera
{
private:
	objMatrices cameraMatrices;

	DirectX::XMFLOAT3 cameraPos;
	DirectX::XMFLOAT3 lookAt;
	DirectX::XMFLOAT3 upVec;
	DirectX::XMFLOAT3 mRight;

	float FOV;
	float ARO;
	float nPlane;
	float fPlane;

public:
	Camera();
	~Camera();

	void createCamera();
	void updateCamera();

	objMatrices getCameraMatrices()const;
	void setCameraMatrices(objMatrices& cameraMatrices);

	void move(DirectX::XMMATRIX& temp);

	void setCameraPos(DirectX::XMFLOAT3 cameraPos);

	DirectX::XMVECTOR getLookAt();
	DirectX::XMVECTOR getUpVec();

	void flushGame();


	void setView(XMMATRIX& view);

	void setLookAtVec(XMFLOAT3 lookAt);
};