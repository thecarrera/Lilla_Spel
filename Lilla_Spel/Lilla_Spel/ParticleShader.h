#ifndef PARTICLESHADER_H
#define PARTICLESHADER_H

#include "Includes.h"

using namespace std;

class ParticleShader
{
private:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX worldM;
		DirectX::XMMATRIX viewM;
		DirectX::XMMATRIX projM;
	};

	ID3D11VertexShader* mVertexShader;
	ID3D11PixelShader* mPixelShader;
	ID3D11InputLayout* mLayout;
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* mSampleSate;

public:
	ParticleShader();
	ParticleShader(const ParticleShader&);
	~ParticleShader();

	void initialize(ID3D11Device* gDevice, HWND hwnd);
	void shutDown();
	void render(ID3D11DeviceContext* gDeviceContext, int indexCount, DirectX::XMMATRIX worldM, DirectX::XMMATRIX viewM, DirectX::XMMATRIX projM);

	bool initializeShader(ID3D11Device* gDevice, HWND hwnd, WCHAR* VSFilename, WCHAR* PSFilename);
	void shutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);
	void setShaderParameters(ID3D11DeviceContext* gdeviceContext, DirectX::XMMATRIX worldM, DirectX::XMMATRIX viewM, DirectX::XMMATRIX projM);
	void renderShader(ID3D11DeviceContext* deviceContext, int indexCount);
};


#endif // !PARTICLESHADER_H
