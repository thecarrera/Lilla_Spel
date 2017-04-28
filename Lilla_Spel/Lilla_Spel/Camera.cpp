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
	this->cameraPos = {0, 40, -20};	// y 50% större än z ger bra-ish
	this->lookAt = { 0, -1, 0.5 };		// lookAT vill vi ska vara på origo, där spelaren är
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
		0,1.0f, 0, 0,
		0, 0, 1.0f, 0,
		0, 0, 0, 1 };

	DirectX::XMMATRIX viewM = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookToLH(cameraPosVec, lookAtVec, upVecVec));
	DirectX::XMMATRIX projM = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(FOV, ARO, nPlane, fPlane));

	XMFLOAT4X4 w;
	XMFLOAT4X4 v;
	XMFLOAT4X4 p;

	XMStoreFloat4x4(&w, worldM);
	XMStoreFloat4x4(&v, viewM);
	XMStoreFloat4x4(&p, projM);

	cout << "Old World: " << endl;

	cout << w._11 << ", " << w._12 << ", " << w._13 << ", " << w._14 << endl;
	cout << w._21 << ", " << w._22 << ", " << w._23 << ", " << w._24 << endl;
	cout << w._31 << ", " << w._32 << ", " << w._33 << ", " << w._34 << endl;
	cout << w._41 << ", " << w._42 << ", " << w._43 << ", " << w._44 << endl;
	cout << "Old View: " << endl;
	cout << endl << v._11 << ", " << v._12 << ", " << v._13 << ", " << v._14 << endl;
	cout << v._21 << ", " << v._22 << ", " << v._23 << ", " << v._24 << endl;
	cout << v._31 << ", " << v._32 << ", " << v._33 << ", " << v._34 << endl;
	cout << v._41 << ", " << v._42 << ", " << v._43 << ", " << v._44 << endl;
	cout << "Old Proj: " << endl;
	cout << endl << p._11 << ", " << p._12 << ", " << p._13 << ", " << p._14 << endl;
	cout << p._21 << ", " << p._22 << ", " << p._23 << ", " << p._24 << endl;
	cout << p._31 << ", " << p._32 << ", " << p._33 << ", " << p._34 << endl;
	cout << p._41 << ", " << p._42 << ", " << p._43 << ", " << p._44 << endl << endl;


	this->cameraMatrices = { (worldM), (viewM), (projM) };
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
	/*std::cout << *cameraMatrices.projM.r->m128_f32 << ", " << *cameraMatrices.projM.r->m128_i16 << ", " << *cameraMatrices.projM.r->m128_i32 << std::endl;
	std::cout << *cameraMatrices.projM.r->m128_i64 << ", " << *cameraMatrices.projM.r->m128_i8 << ", " << *cameraMatrices.projM.r->m128_u16 << std::endl;
	std::cout << *cameraMatrices.projM.r->m128_u32 << ", " << *cameraMatrices.projM.r->m128_u64 << ", " << *cameraMatrices.projM.r->m128_u8 << std::endl << std::endl;
*/
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

DirectX::XMVECTOR Camera::getLookAt()
{
	DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&this->lookAt);
	return vec;
}
DirectX::XMVECTOR Camera::getUpVec()
{
	DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&this->upVec);
	return vec;
}