#include "Camera.h"

Camera::Camera()
{
	createCamera();
}
Camera::~Camera()
{

}

void Camera::createCamera()
{
	this->cameraPos = { 0, 50, -25 };	// y 50% större än z ger bra-ish
	this->lookAt = { 0, 0, 1 };		// lookAT vill vi ska vara på origo, där spelaren är
	this->upVec = { 0, 1, 0 };

	DirectX::XMVECTOR cameraPosVec = DirectX::XMLoadFloat3(&this->cameraPos);
	DirectX::XMVECTOR lookAtVec = DirectX::XMLoadFloat3(&this->lookAt);
	DirectX::XMVECTOR upVecVec = DirectX::XMLoadFloat3(&this->upVec);
	DirectX::XMVECTOR mRightVec;

	mRightVec = DirectX::XMVector3Cross(upVecVec, lookAtVec);
	mRightVec = DirectX::XMVector3Normalize(mRightVec);

	DirectX::XMStoreFloat3(&this->mRight, mRightVec);

	this->FOV = { 0.45f * DirectX::XM_PI };
	this->ARO = (float)WIDTH / (float)HEIGHT;
	this->nPlane = 0.1f;
	this->fPlane = 200.0f;
	 
	DirectX::XMMATRIX worldM =
	{ 1.0f, 0, 0, 0,
		0, 1.0f, 0, 0,
		0, 0, 1.0f, 0,
		0, 0, 0, 1 };

	DirectX::XMMATRIX viewM = DirectX::XMMatrixLookAtLH(cameraPosVec, lookAtVec, upVecVec);
	DirectX::XMMATRIX projM = DirectX::XMMatrixPerspectiveFovLH(FOV, ARO, nPlane, fPlane);

	this->cameraMatrices = { DirectX::XMMatrixTranspose(worldM), DirectX::XMMatrixTranspose(viewM), DirectX::XMMatrixTranspose(projM) };
}
void Camera::updateCamera()
{
	DirectX::XMVECTOR cameraPosVec = DirectX::XMLoadFloat3(&this->cameraPos);
	DirectX::XMVECTOR lookAtVec = DirectX::XMLoadFloat3(&this->lookAt);
	DirectX::XMVECTOR upVecVec = DirectX::XMLoadFloat3(&this->upVec);
	DirectX::XMVECTOR mRightVec = DirectX::XMLoadFloat3(&this->mRight);


	DirectX::XMMATRIX viewM = DirectX::XMMatrixLookAtLH(cameraPosVec, lookAtVec, upVecVec);
	
	DirectX::XMMATRIX projM = DirectX::XMMatrixPerspectiveFovLH(FOV, ARO, nPlane, fPlane);

	this->cameraMatrices = { this->cameraMatrices.worldM, DirectX::XMMatrixTranspose(viewM), DirectX::XMMatrixTranspose(projM) };
}
objMatrices Camera::getCameraMatrices()const
{
	return this->cameraMatrices;
}
void Camera::setCameraMatrices(objMatrices cameraMatrices)
{
	this->cameraMatrices = cameraMatrices;
}

void Camera::move(DirectX::XMMATRIX temp)
{
	this->cameraMatrices.viewM *= temp;
}
void Camera::setCameraPos(DirectX::XMFLOAT3 cameraPos)
{
	this->cameraPos = cameraPos;
}