#include "ParticleShader.h"

ParticleShader::ParticleShader()
{
	this->mVertexShader = 0;
	this->mPixelShader = 0;
	this->mLayout = 0;
	this->matrixBuffer = 0;
	this->mSampleSate = 0;
}

ParticleShader::ParticleShader(const ParticleShader& other)
{

}

ParticleShader::~ParticleShader()
{

}

void ParticleShader::initialize(ID3D11Device* gDevice, HWND hwnd)
{
	this->initializeShader(gDevice, hwnd, L"../Lillaspel/VSParticle.hlsl", L"../Lillaspel/PSParticle.hlsl");
}

void ParticleShader::shutDown()
{
	this->shutdownShader();
}

void ParticleShader::render(ID3D11DeviceContext* gDeviceContext, int indexCount, DirectX::XMMATRIX worldM, DirectX::XMMATRIX viewM, DirectX::XMMATRIX projM)
{
	this->setShaderParameters(gDeviceContext, worldM, viewM, projM);
}

bool ParticleShader::initializeShader(ID3D11Device * gDevice, HWND hwnd, WCHAR * VSFilename, WCHAR * PSFilename)
{
	ID3D10Blob* errorMsg;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	//D3D11_SAMPLER_DESC samplerDesc; //för texturer right?

	errorMsg = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	HRESULT result;
	result = D3DCompileFromFile(VSFilename, NULL, NULL, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMsg); //VARNINGGGGGG
	//DENNA ÄR ÄNDRAD FRÅN EXEMPLET

	if (FAILED(result))
	{
		//failed to compile
		if (errorMsg)
		{
		//	OutputShaderErrorMessage(errorMessage, hwnd, VSFilename); //hmm
		}
		//couldnt find the shader
		else
		{
		//	MessageBox(hwnd, VSFilename, L"Missing Shader File", MB_OK);	//HMMMMM
		}

		return false;
	}

	result = D3DCompileFromFile(PSFilename, NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMsg); //HMMMMMMMMMMMMM

	if (FAILED(result))
	{
		//failed to compile
		if (errorMsg)
		{
		//	OutputShaderErrorMessage(errorMessage, hwnd, PSFilename); //hmm
		}
		//couldnt find the shader
		else
		{
		//	MessageBox(hwnd, PSFilename, L"Missing Shader File", MB_OK);	//HMMMMM
		}

		return false;
	}

	//vertex shader från buffer
	result = gDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &this->mVertexShader);
	if (FAILED(result))
	{
		return false;
	}

	//pixel shader från buffer
	result = gDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &this->mPixelShader);
	if (FAILED(result))
	{
		return false;
	}

	//vertex input layout desc
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	//count of element in the layout
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[10]);

	//create vertex input layout
	result = gDevice->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &this->mLayout);
	if (FAILED(result))
	{
		return false;
	}

	//Releasa alla buffrar för de behövs inte längre
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;
	
	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	//desc för dynamisk matrix konstant buffer som är i vertex shadern!!!
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = gDevice->CreateBuffer(&matrixBufferDesc, NULL, &this->matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//textur, Nej inte för mig
	return true;
}

void ParticleShader::shutdownShader()
{
	if (this->mSampleSate)
	{
		this->mSampleSate->Release();
		this->mSampleSate = 0;
	}

	if (this->matrixBuffer)
	{
		this->matrixBuffer->Release();
		this->matrixBuffer = 0;
	}

	if (this->mLayout)
	{
		this->mLayout->Release();
		this->mLayout = 0;
	}

	if (this->mPixelShader)
	{
		this->mPixelShader->Release();
		this->mPixelShader = 0;
	}

	if (this->mVertexShader)
	{
		this->mVertexShader->Release();
		this->mVertexShader = 0;
	}
}

void ParticleShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	compileErrors = (char*)(errorMessage->GetBufferPointer());

	bufferSize = errorMessage->GetBufferSize();

	fout.open("shader-error.txt");

	for (int i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	fout.close();

	errorMessage->Release();
	errorMessage = 0;

	//MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message.", shaderFilename, MB_OK); //hmm
}

void ParticleShader::setShaderParameters(ID3D11DeviceContext* gdeviceContext, DirectX::XMMATRIX worldM, DirectX::XMMATRIX viewM, DirectX::XMMATRIX projM)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	//trannsponera alla matriser
	DirectX::XMMatrixTranspose(worldM);
	DirectX::XMMatrixTranspose(viewM);
	DirectX::XMMatrixTranspose(viewM);

	gdeviceContext->Map(this->matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	//kopiera matriserna in i konstanta buffen
	dataPtr->worldM = worldM;
	dataPtr->viewM = viewM;
	dataPtr->projM = projM;

	gdeviceContext->Unmap(this->matrixBuffer, 0);

	bufferNumber = 0;

	gdeviceContext->VSSetConstantBuffers(bufferNumber, 1, &this->matrixBuffer);

	//ingen shader resource
}

void ParticleShader::renderShader(ID3D11DeviceContext* gDeviceContext, int indexCount)
{
	gDeviceContext->IASetInputLayout(this->mLayout);

	gDeviceContext->VSSetShader(this->mVertexShader, NULL, 0);
	gDeviceContext->PSSetShader(this->mPixelShader, NULL, 0);

	//ingen sampler
	
	gDeviceContext->DrawIndexed(indexCount, 0, 0);
}
