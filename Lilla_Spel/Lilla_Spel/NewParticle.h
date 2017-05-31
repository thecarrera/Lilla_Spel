#ifndef NEWPARTICLE_H
#define NEWPARTICLE_H
#include "Includes.h"
#include "Camera.h"

class NewParticle
{
private:
	//matriserna
	struct matrixStruct
	{
		DirectX::XMMATRIX worldM;
		DirectX::XMMATRIX viewM;
		DirectX::XMMATRIX projM;
	};
	matrixStruct matriser;

	//punkten där grejen skall göras
	struct TriangleVertex
	{
		float x, y, z;
	};

	//devices med & ska skickas in till alla funktioner som vill använda dem.

	//buffer
	ID3D11Buffer* newVertexBuffer;
	//konstant buffer
	ID3D11Buffer* GSCONSTANTBUFFER;

	//shaders
	ID3D11InputLayout* newVertexLayout;
	ID3D11VertexShader* newVertexShader;
	ID3D11GeometryShader* newGeometryShader; //GS GÖR FÖRSTA
	ID3D11PixelShader* newPixelShader;

	//clear background
	/*ID3D11RenderTargetView* gBackbufferRTV;
	ID3D11DepthStencilView* pDSV;*/
public:
	NewParticle();
	~NewParticle();

	void createShaders(ID3D11Device* gDevice);
	D3D11_SUBRESOURCE_DATA initiateSubresources();
	void initiateMatrices();
	void updateMatrices(ID3D11DeviceContext* gDeviceContext, Camera* camera); //använda för initiate med
	ID3D11Buffer* createConstantBuffer(ID3D11Device* gDevice, UINT size, bool dynamic, bool CPUupdates, D3D11_SUBRESOURCE_DATA* pData);
	void createTriangleData(ID3D11Device* gDevice);
	void renderParticles(ID3D11DeviceContext* gDeviceContext);
	void make(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext);
	void update(ID3D11DeviceContext* gDeviceContext, Camera* camera);
	void release();
};

#endif // !NEWPARTICLE_H
