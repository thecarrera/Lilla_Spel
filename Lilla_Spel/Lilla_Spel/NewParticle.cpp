#include "NewParticle.h"

NewParticle::NewParticle()
{
	//buffer
	this->newVertexBuffer = nullptr;
	//konstanta buffers
	this->GSCONSTANTBUFFER = nullptr;

	//shaders
	this->newVertexLayout = nullptr;
	this->newVertexShader = nullptr;
	this->newGeometryShader = nullptr;
	this->newPixelShader = nullptr;

	//clear background
	/*this->gBackbufferRTV = nullptr;
	this->pDSV = nullptr;*/
}

NewParticle::~NewParticle()
{

}

void NewParticle::createShaders(ID3D11Device* gDevice)
{
	//Vertex Shadern

	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(
		L"NewParticleVertexShader.hlsl",
		nullptr,
		nullptr,
		"VS_main",
		"vs_5_0",
		0,
		0,
		&pVS,
		nullptr
	);

	gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &this->newVertexShader);

	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &this->newVertexLayout);
	//sen så ska blobben bort
	pVS->Release();

	//Geometry Shadern

	ID3DBlob* pGS = nullptr;
	D3DCompileFromFile(
		L"NewParticleGeometryShader.hlsl",
		nullptr,
		nullptr,
		"GS_main",
		"gs_5_0",
		0,
		0,
		&pGS,
		nullptr
	);

	gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &this->newGeometryShader);
	//blobb bort
	pGS->Release();

	//Pixel Shadern

	ID3DBlob* pPS = nullptr;
	D3DCompileFromFile(
		L"NewParticlePixelShader.hlsl",
		nullptr,
		nullptr,
		"PS_main",
		"ps_5_0",
		0,
		0,
		&pPS,
		nullptr
	);

	gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &this->newPixelShader);
	//blobb bort
	pPS->Release();
}

D3D11_SUBRESOURCE_DATA NewParticle::initiateSubresources()
{
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &this->matriser;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;
	return data;
}

void NewParticle::initiateMatrices()
{
	matriser.worldM = DirectX::XMMATRIX();
	matriser.viewM = DirectX::XMMATRIX();
	matriser.projM = DirectX::XMMATRIX();
}

void NewParticle::updateMatrices(ID3D11DeviceContext* gDeviceContext, Camera* camera)
{
	matriser.worldM = camera->getCameraMatrices().worldM;
	matriser.viewM = camera->getCameraMatrices().viewM;
	matriser.projM = camera->getCameraMatrices().projM;

	D3D11_MAPPED_SUBRESOURCE pData;
	//updatera constantbuffern
	gDeviceContext->Map(this->GSCONSTANTBUFFER, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);

	memcpy(pData.pData, &matriser, sizeof(matrixStruct));

	gDeviceContext->Unmap(this->GSCONSTANTBUFFER, 0);

	gDeviceContext->GSSetConstantBuffers(0, 1, &this->GSCONSTANTBUFFER);
}

ID3D11Buffer* NewParticle::createConstantBuffer(ID3D11Device* gDevice, UINT size, bool dynamic, bool CPUupdates, D3D11_SUBRESOURCE_DATA * pData)
{
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = size;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	//välj det som man vill ha
	if (dynamic && CPUupdates)
	{
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	ID3D11Buffer* pBuffer = 0;
	gDevice->CreateBuffer(&desc, pData, &pBuffer);

	return(pBuffer);
}

void NewParticle::createTriangleData(ID3D11Device* gDevice)
{
	TriangleVertex triangleVertices[3] =
	{
		-0.5f, 0.5f, 0.0f,	//v0 pos	//första triangel 

		0.5f, 0.5f, 0.0f,	//v1

		-0.5f, -0.5f, 0.0f,  //v2
	};

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc)); //zeroar
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(triangleVertices);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = triangleVertices;
	gDevice->CreateBuffer(&bufferDesc, &data, &this->newVertexBuffer);
}

void NewParticle::renderParticles(ID3D11DeviceContext* gDeviceContext)
{
	//borde cleara allt
	float clearColor[] = { 0, 0, 0, 1 };

	gDeviceContext->VSSetShader(this->newVertexShader, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(this->newGeometryShader, nullptr, 0);
	gDeviceContext->PSSetShader(this->newPixelShader, nullptr, 0);

	UINT32 vertexSize = sizeof(float) * 5; //venne kanske nåt annat istället för 5
	UINT32 offset = 0;
	gDeviceContext->IASetVertexBuffers(0, 1, &this->newVertexBuffer, &vertexSize, &offset);

	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gDeviceContext->IASetInputLayout(this->newVertexLayout);

	gDeviceContext->Draw(3, 0);
}

void NewParticle::make(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext)
{
	this->createShaders(gDevice);	//4. Skapa vertex- och pixel-shaders

	this->createTriangleData(gDevice);	 //5. Definiera triangelvertiser, 
										 //6. Skapa vertex buffer, 
										 //7. Skapa input layout

	this->initiateMatrices();
	D3D11_SUBRESOURCE_DATA data = this->initiateSubresources();
	this->GSCONSTANTBUFFER = createConstantBuffer(gDevice, sizeof(matrixStruct), true, true, &data);
	gDeviceContext->GSSetConstantBuffers(0, 1, &this->GSCONSTANTBUFFER);
}

void NewParticle::update(ID3D11DeviceContext* gDeviceContext, Camera* camera)
{
	this->renderParticles(gDeviceContext);	//8. Rendera

	this->updateMatrices(gDeviceContext, camera); //9. uppdatera
}

void NewParticle::release()
{
	this->newVertexBuffer->Release();
	this->newVertexLayout->Release();
	this->newVertexShader->Release();

	this->newGeometryShader->Release();

	this->newPixelShader->Release();
}