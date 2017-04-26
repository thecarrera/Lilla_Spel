#include "DirectX.h"

/*
#################################################################################################################################
#																																#
#							All Direct X components, mostly the core engine of the pipeline.									#
#							Allowing us to add key techniques as modules.														#
#																																#
#################################################################################################################################
*/

DX::DX()
{
}
DX::~DX()
{
	this->Clean();
}
void DX::Clean()
{
	SAFE_RELEASE(this->gDevice);
	SAFE_RELEASE(this->gDeviceContext);
	SAFE_RELEASE(this->gSwapChain);
	SAFE_RELEASE(this->gBackBufferRTV);

	SAFE_RELEASE(this->gVertexLayout);
	SAFE_RELEASE(this->gVertexBuffer);
	SAFE_RELEASE(this->gVertexShader);

	SAFE_RELEASE(this->gGeometryShader);
	SAFE_RELEASE(this->gFragmentShader);

	SAFE_RELEASE(this->depthState);
	SAFE_RELEASE(this->gDSV);
	SAFE_RELEASE(this->gDepthStencil);

	SAFE_RELEASE(this->gCBuffer);
	SAFE_RELEASE(this->shaderBuffer);
	SAFE_RELEASE(this->samplerState);

	SAFE_RELEASE(this->gTextureRTV);
}

void DX::OfflineCreation(HMODULE hModule, HWND* wndHandle)
{
	this->CreateDirect3DContext(wndHandle);

	this->SetViewport();
	
	//clock
	this->frameTime = time(0);

	//this->linker.LoadModel("Ogre.obj", this->gDevice, this->gVertexBuffer, this->shaderBuffer);

	//PARTICLE!!!!!
	this->initiateParticles(*wndHandle);


	this->player = new Player();
	DirectX::XMMATRIX world =
	{ 1.0f, 0, 0, 0,
		0, 1.0f, 0, 0,
		0, 0, 1.0f, 0,
		0, 0, 0, 1 };
	//world = DirectX::XMMatrixTranspose(world); //unnecesarry in this tilfälle

	this->createGCBuffer(); //kamera

	this->camera = new Camera();

	this->player->initiateMatrices(world, this->camera->getCameraMatrices().viewM, this->camera->getCameraMatrices().projM);

	//this->linker.Texture(this->gDevice, this->gDeviceContext, this->gTextureRTV);

	this->CreateShaders();

	Vertex** vtx = CreateTriangleData(this->gDevice, this->gVertexBufferArray, //vertexbuffarray array med verticernas arrayer
		this->vertexCountOBJ, this->gVertexBuffer2_size, this->objCoords);
	
	
}

void DX::initiateParticles(HWND hwnd)
{
	this->mParticleSystem = 0;
	this->mParticleShader = 0;

	//PARTIKEL SHADERN!!!
	//creatar partikel shader objektet
	this->mParticleShader = new ParticleShader();

	//initializar particle shader objektet
	this->mParticleShader->initialize(this->gDevice, hwnd);

	//PARTIKEL SYSTEMET!!!
	this->mParticleSystem = new ParticleClass();

	this->mParticleSystem->initialize(this->gDevice);
}

//Flytta detta och blobsen till particle klassen right? 
void DX::RenderParticles()
{
	this->mParticleSystem->render(this->gDeviceContext);

	this->mParticleShader->render(this->gDeviceContext, this->mParticleSystem->getmIndexCount(),
		this->camera->getCameraMatrices().worldM, this->camera->getCameraMatrices().viewM,
		this->camera->getCameraMatrices().projM);
	//MATRISERNA KAN VARA FEL


}

//SHUTDOWN KAN VARA BRA ATT LÄGGA IN MED

void DX::Update()
{
	//this->player->updateConstantBuffer(this->gCBuffer);
	
	//PARTICLE!!!!
	this->mParticleSystem->frameUpdate(this->frameTime, this->gDeviceContext);

	
	player->move(this->camera);

	float clearColor[] = { 0.3f, 0.0f, 0.5f, 1.f };
	player->setWorldM(this->camera->getCameraMatrices().worldM);
	//player->setViewM(this->camera->getCameraMatrices().viewM);
	player->setProjectionM(this->camera->getCameraMatrices().projM);

	this->gDeviceContext->ClearRenderTargetView(this->gBackBufferRTV, clearColor);
	this->gDeviceContext->ClearDepthStencilView(this->gDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	this->updatePlayerConstantBuffer(); //annars ser inte rör
	this->Render(true);

	//updateraKamera

	this->resetConstantBuffer();
	this->Render(false);

	//en till render?

	//en renderloop för spelaren och en för resten, bool beroende på vart i arrayen vi är
	//this->Render();
	this->gSwapChain->Present(1, 0);
}

void DX::CreateDirect3DContext(HWND* wndHandle)
{
	CoInitialize(NULL);

	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = *wndHandle;
	scd.BufferDesc.Width = WIDTH;
	scd.BufferDesc.Height = HEIGHT;
	scd.SampleDesc.Count = PIXELSAMPLE;
	scd.Windowed = TRUE;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&this->gSwapChain,
		&this->gDevice,
		NULL,
		&this->gDeviceContext);

	if (SUCCEEDED(hr))
	{
		ID3D11Texture2D* pBackBuffer = nullptr;
		this->gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		this->DepthBuffer();

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;

		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		hr = gDevice->CreateDepthStencilView(this->gDepthStencil, nullptr, &this->gDSV);

		hr = gDevice->CreateRenderTargetView(pBackBuffer, NULL, &this->gBackBufferRTV);

		pBackBuffer->Release();

		this->gDeviceContext->OMSetRenderTargets(1, &this->gBackBufferRTV, this->gDSV);
	}
}
void DX::SetViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width = WIDTH;
	vp.Height = HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	this->gDeviceContext->RSSetViewports(1, &vp);
}
void DX::Render(bool isPlayer) 
{	
	UINT32 vertexSize = sizeof(float) * 5;
	UINT32 offset = 0;

	this->gDeviceContext->IASetInputLayout(this->gVertexLayout);

	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->gDeviceContext->VSSetShader(this->gVertexShader, nullptr, 0);

	this->gDeviceContext->GSSetShader(this->gGeometryShader, nullptr, 0);

	this->gDeviceContext->PSSetShader(this->gFragmentShader, nullptr, 0);

	this->gDeviceContext->PSSetShaderResources(0, 1, &this->gTextureRTV);
	this->gDeviceContext->PSSetSamplers(0, 1, &this->samplerState);

	//något mer här?

	//D3D11_MAPPED_SUBRESOURCE dataPtr;
	//this->gDeviceContext->Map(this->gCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);

	//this->cameraMatrices.worldM *= DirectX::XMMatrixRotationY(-0.02f); 
	////cameraMatrix.worldM *= DirectX::XMMatrixRotationX(-0.02f);

	//memcpy(dataPtr.pData, &this->cameraMatrices, sizeof(this->cameraMatrices));

	//this->gDeviceContext->Unmap(this->gCBuffer, 0);




	this->gDeviceContext->GSSetConstantBuffers(0, 1, &this->gCBuffer);
	this->gDeviceContext->PSSetConstantBuffers(0, 1, &this->shaderBuffer);

	if (isPlayer == true)
	{
		this->gDeviceContext->IASetVertexBuffers(0, 1, &this->gVertexBufferArray[0], &vertexSize, &offset);
		this->gDeviceContext->Draw(this->vertexCountOBJ[0], 0);
	}

	if (isPlayer == false)
	{
		this->RenderParticles();

		for (int i = 1; i < this->gVertexBuffer2_size; i++) {
			this->gDeviceContext->IASetVertexBuffers(0, 1, &this->gVertexBufferArray[i], &vertexSize, &offset); //första parametern 0 egentligen, 1 pga en partikel vertex buffern innan?
			this->gDeviceContext->Draw(this->vertexCountOBJ[i], 0);
		}
	}
}

void DX::CreateShaders()
{
	HRESULT hr;

	ID3DBlob* error = nullptr;

	//Vertex Shader
	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(
		L"VertexShader.hlsl",
		nullptr,
		nullptr,
		"VS_main",
		"vs_5_0",
		0,
		0,
		&pVS,
		&error
	);
	hr = this->gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), NULL, &this->gVertexShader);

	if (error)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 } 
	};


	this->gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &this->gVertexLayout);


	SAFE_RELEASE(pVS);

	//Geometric Shader
	ID3DBlob* pGS = nullptr;

	D3DCompileFromFile(
		L"GeometryShader.hlsl",
		nullptr,
		nullptr,
		"GS_main",
		"gs_5_0",
		0,
		0,
		&pGS,
		&error
	);
	this->gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), NULL, &this->gGeometryShader);

	if (error)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	SAFE_RELEASE(pGS);

	//Fragment Shader
	ID3DBlob* pFS = nullptr;
	D3DCompileFromFile(
		L"PixelShader.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"FS_main",		// entry point
		"ps_5_0",		// shader model (target)
		0,				// shader compile options
		0,				// effect compile options
		&pFS,			// double pointer to ID3DBlob		
		&error			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	hr = this->gDevice->CreatePixelShader(pFS->GetBufferPointer(), pFS->GetBufferSize(), nullptr, &this->gFragmentShader);

	if (error)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	SAFE_RELEASE(pFS);

	//Particle Vertex
	ID3DBlob* VSPBlob = nullptr;
	hr = D3DCompileFromFile(
		L"VSParticle.hlsl",
		nullptr,
		nullptr,
		"main",
		"vs_5_0",
		0,
		0,
		&VSPBlob,
		&error);

	hr = this->gDevice->CreateVertexShader(VSPBlob->GetBufferPointer(), VSPBlob->GetBufferSize(), nullptr, &this->gVSParticle);

	if (error)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	SAFE_RELEASE(VSPBlob);

	//Particle Geometry
	ID3DBlob* GSPBlob = nullptr;
	hr = D3DCompileFromFile(
		L"GSParticle.hlsl",
		nullptr,
		nullptr,
		"main",
		"gs_5_0",
		0,
		0,
		&GSPBlob,
		&error);

	hr = this->gDevice->CreateGeometryShader(GSPBlob->GetBufferPointer(), GSPBlob->GetBufferSize(), nullptr, &this->gGSParticle);

	if (error)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	SAFE_RELEASE(GSPBlob);

	//Particle Geometry
	ID3DBlob* PSPBlob = nullptr;
	hr = D3DCompileFromFile(
		L"PSParticle.hlsl",
		nullptr,
		nullptr,
		"main",
		"ps_5_0",
		0,
		0,
		&PSPBlob,
		&error);

	hr = this->gDevice->CreatePixelShader(PSPBlob->GetBufferPointer(), PSPBlob->GetBufferSize(), nullptr, &this->gPSParticle);

	if (error)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	SAFE_RELEASE(PSPBlob);



	SAFE_RELEASE(error);

	//Sampler State
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.MipLODBias = 0;
	sampDesc.MaxAnisotropy = PIXELSAMPLE;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = 0;

	hr = this->gDevice->CreateSamplerState(&sampDesc, &this->samplerState);
}
void DX::createGCBuffer()
{
	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cBufferDesc.ByteWidth = sizeof(objMatrices);
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cBufferDesc.MiscFlags = 0;
	cBufferDesc.StructureByteStride = 0;

	HRESULT hr = 0;
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &this->player->getMatrices();

	hr = gDevice->CreateBuffer(&cBufferDesc, &data, &this->gCBuffer);

	if (FAILED(hr))
	{
		exit(-1);
	}
}
void DX::DepthBuffer()
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC dBufferDesc;
	dBufferDesc.Width = (UINT)WIDTH;
	dBufferDesc.Height = (UINT)HEIGHT;
	dBufferDesc.MipLevels = 1;
	dBufferDesc.ArraySize = 1;
	dBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dBufferDesc.SampleDesc.Count = PIXELSAMPLE;
	dBufferDesc.SampleDesc.Quality = 0;
	dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	dBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dBufferDesc.CPUAccessFlags = 0;
	dBufferDesc.MiscFlags = 0;

	hr = gDevice->CreateTexture2D(&dBufferDesc, NULL, &this->gDepthStencil);

	if (FAILED(hr))
	{
		return exit(-1);
	}
}

void DX::updatePlayerConstantBuffer() //med player matriser
{
	objMatrices playerMatrices = this->player->getMatrices();

	D3D11_MAPPED_SUBRESOURCE dataPtr;

	//Låser buffern för GPU:n och hämtar den till CPU
	this->gDeviceContext->Map(this->gCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);

	//do stuff, just setting the matrices right

	memcpy(dataPtr.pData, &playerMatrices, sizeof(playerMatrices)); //SJÄLVA UPPDTAERINGEN MED NYA MATRICE

	//Ger GPU:n tillgång till datan igen
	this->gDeviceContext->Unmap(this->gCBuffer, 0);
}

//fixa med kamera

void DX::updateCameraConstantBuffer() //används inte än
{
	objMatrices cameraMatrices = this->camera->getCameraMatrices();

	D3D11_MAPPED_SUBRESOURCE dataPtr;

	//Låser buffern för GPU:n och hämtar den till CPU
	this->gDeviceContext->Map(this->gCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);

	//do stuff 

	memcpy(dataPtr.pData, &cameraMatrices, sizeof(cameraMatrices)); //SJÄLVA UPPDTAERINGEN MED NYA MATRICE

	//Ger GPU:n tillgång till datan igen
	this->gDeviceContext->Unmap(this->gCBuffer, 0);
}

void DX::resetConstantBuffer()
{
	objMatrices standardMatrices;

	standardMatrices.worldM =
	{ 1.0f,0,0,0,
		0,1.0f,0,0,
		0,0,1.0f,0,
		0,0,0,1 };

	standardMatrices.viewM = this->camera->getCameraMatrices().viewM;

	standardMatrices.projM = this->camera->getCameraMatrices().projM;

	standardMatrices = { DirectX::XMMatrixTranspose(standardMatrices.worldM), standardMatrices.viewM, standardMatrices.projM };

	D3D11_MAPPED_SUBRESOURCE dataPtr;
	ZeroMemory(&dataPtr, sizeof(D3D11_MAPPED_SUBRESOURCE)); //behövs imnte

	//Låser buffern för GPU:n och hämtar den till CPU
	this->gDeviceContext->Map(this->gCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);

	memcpy(dataPtr.pData, &standardMatrices, sizeof(standardMatrices)); //SJÄLVA UPPDTAERINGEN MED NYA MATRICE

	//Ger GPU:n tillgång till datan igen
	this->gDeviceContext->Unmap(this->gCBuffer, 0);
}