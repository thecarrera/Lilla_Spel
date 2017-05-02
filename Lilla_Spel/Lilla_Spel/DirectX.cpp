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

	SAFE_DELETE(this->gVertexBufferArray);
	
}

void DX::OfflineCreation(HMODULE hModule, HWND* wndHandle)
{
	this->CreateDirect3DContext(wndHandle);

	this->SetViewport();

	this->FBX.Import("test.gay", this->gDevice, this->gVertexBufferArray);
	this->gVertexBufferArray_size = FBX.getTotalMeshes();

	col = Collision(this->FBX.getMeshes(), FBX.getMeshCount());

	this->player = new Player();
	DirectX::XMMATRIX world =
	{ 1.0f, 0, 0, 0,
		0, 1.0f, 0, 0,
		0, 0, 1.0f, 0,
		0, 0, 0, 1 };

	this->createCBuffer(); //kamera

	this->camera = new Camera();

	this->player->initiateMatrices(world, this->camera->getCameraMatrices().viewM, this->camera->getCameraMatrices().projM);

	this->CreateShaders();

	this->createMenu();
	//Vertex** vtx = CreateTriangleData(this->gDevice, this->gVertexBufferArray,
	//	this->vertexCountOBJ, this->gVertexBuffer2_size, this->objCoords);

}
void DX::createMenu()
{
	struct Vertex
	{
		float pos[3];
		float uv[2];
		float normal[3];
	};

	Vertex verticies[66] = 
	{

		//Background
		-40.0f, 0.0f, 32.0f,			//Position
		0.0f, 0.0f,						//uv
		0.0f, 1.0f, 0.0f,				//Normal

		40.0f, 0.0f, -32.0f,			//Position
		0.0f, 0.0f,						//uv
		0.0f, 1.0f, 0.0f,				//Normal

		-40.0f, 0.0f, -32.0f,			//Position
		0.0f, 0.0f,						//uv
		0.0f, 1.0f, 0.0f,				//Normal

		-40.0f, 0.0f, 32.0f,				//Position
		0.0f, 0.0f,						//uv
		0.0f, 1.0f, 0.0f,				//Normal

		40.0f, 0.0f, 32.0f,				//Position
		0.0f, 0.0f,						//uv
		0.0f, 1.0f, 0.0f,				//Normal

		40.0f, 0.0f, -32.0f,			//Position
		0.0f, 0.0f,						//uv
		0.0f, 1.0f, 0.0f,				//Normal
	};

	DirectX::XMVECTOR cameraPosVec = { 0, 50, 0 };
	DirectX::XMVECTOR lookAtVec = {0, -1, -0.00000000000000000001f};
	DirectX::XMVECTOR upVecVec = {0, 1, 0};

	float nPlane = 0.1f;
	float fPlane = 200.0f;
	float factor = 0.1f;

	DirectX::XMMATRIX worldM = DirectX::XMMatrixIdentity();

	DirectX::XMMATRIX viewM = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookToLH(cameraPosVec, lookAtVec, upVecVec));
	DirectX::XMMATRIX projM = DirectX::XMMatrixTranspose(DirectX::XMMatrixOrthographicLH(800.0f * factor , 640.0f * factor, nPlane, fPlane));

	this->menuMats.worldM = worldM;
	this->menuMats.viewM = viewM;
	this->menuMats.projM = projM;

	this->printMatrices(this->menuMats);

	D3D11_MAPPED_SUBRESOURCE dataPtr;

	this->gDeviceContext->Map(this->menuBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);

	memcpy(dataPtr.pData, &this->menuMats, sizeof(objMatrices));

	this->gDeviceContext->Unmap(this->menuBuffer, 0);

	HRESULT hr;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex) * 36;// fill

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = &verticies;//fill;
	hr = gDevice->CreateBuffer(&bufferDesc, &data, &gMenuVertexArray);

	if (FAILED(hr))
	{
		exit(-1);
	}
}
void DX::Update()
{
	if (this->menuMsg == false)
	{
		//this->player->updateConstantBuffer(this->gCBuffer);
		player->move(this->camera, col.calculateCollisionData(player->getMatrices().worldM, this->player->getIsDigging()), this->menuMsg, this->tButtonPress, this->lTimePress);
		/*if(col.TestCollision(player->getMatrices().worldM)){
			cout << "Collision!!" << endl;
		}*/

		this->clearRender();
		interactiveCol.test(col.getCollisionData());

		this->updatePlayerConstantBuffer(); //annars ser inte rör

		this->Render(true);

		//updateraKamera

		this->resetConstantBuffer();
		this->Render(false);

		this->updateCameraConstantBuffer();
	}
	else
	{
		this->clearRender();
		this->menuControls();
		this->renderInGameMenu();
	}
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
	UINT32 vertexSize = sizeof(float) * 8;
	UINT32 offset = 0;

	this->gDeviceContext->IASetInputLayout(this->gVertexLayout);

	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->gDeviceContext->VSSetShader(this->gVertexShader, nullptr, 0);
	this->gDeviceContext->GSSetShader(this->gGeometryShader, nullptr, 0);
	this->gDeviceContext->PSSetShader(this->gFragmentShader, nullptr, 0);
	this->gDeviceContext->PSSetShaderResources(0, 1, &this->gTextureRTV);
	this->gDeviceContext->PSSetSamplers(0, 1, &this->samplerState);

	this->gDeviceContext->GSSetConstantBuffers(0, 1, &this->gCBuffer);
	this->gDeviceContext->PSSetConstantBuffers(0, 1, &this->shaderBuffer);

	this->gVertexBufferArray_size = FBX.getTotalMeshes();

	if (isPlayer == true)
	{
		this->gDeviceContext->IASetVertexBuffers(0, 1, &this->gVertexBufferArray[5], &vertexSize, &offset);
		this->gDeviceContext->Draw(FBX.getPlayerSumVertices() , 0);
	}

	if (isPlayer == false)
	{
		for (int i = 6; i < this->gVertexBufferArray_size; i++) {
			if (FBX.getMeshBoundingBox(i) == 0)
			{
				this->gDeviceContext->IASetVertexBuffers(0, 1, &this->gVertexBufferArray[i], &vertexSize, &offset);
				this->gDeviceContext->Draw(FBX.getSumVertices(), 0);
			}
		}
	}
}
void DX::clearRender()
{
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.f };

	this->gDeviceContext->ClearRenderTargetView(this->gBackBufferRTV, clearColor);
	this->gDeviceContext->ClearDepthStencilView(this->gDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
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
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
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
		L"PixelShader.hlsl",
		nullptr,		
		nullptr,		
		"FS_main",		
		"ps_5_0",		
		0,				
		0,				
		&pFS,			
		&error			
	);

	hr = this->gDevice->CreatePixelShader(pFS->GetBufferPointer(), pFS->GetBufferSize(), nullptr, &this->gFragmentShader);

	if (error)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	SAFE_RELEASE(pFS);
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
void DX::createCBuffer()
{
	D3D11_BUFFER_DESC mBufferDesc;
	mBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	mBufferDesc.ByteWidth = sizeof(objMatrices);
	mBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	mBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mBufferDesc.MiscFlags = 0;
	mBufferDesc.StructureByteStride = 0;

	HRESULT hr = 0;
	D3D11_SUBRESOURCE_DATA mdata;
	mdata.pSysMem = &this->player->getMatrices();

	hr = gDevice->CreateBuffer(&mBufferDesc, &mdata, &this->menuBuffer);

	if (FAILED(hr))
	{
		exit(-1);
	}

	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cBufferDesc.ByteWidth = sizeof(objMatrices);
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cBufferDesc.MiscFlags = 0;
	cBufferDesc.StructureByteStride = 0;
	
	hr = 0;
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
	dBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
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

	TEMP.worldM = this->player->getMatrices().worldM;
	TEMP.viewM = camera->getCameraMatrices().viewM;
	TEMP.projM = camera->getCameraMatrices().projM;

	objMatrices playerMatrices = TEMP; //this->player->getMatrices();

	XMFLOAT4X4 temp;
	XMStoreFloat4x4(&temp, this->player->getMatrices().worldM);

	D3D11_MAPPED_SUBRESOURCE dataPtr;

	this->gDeviceContext->Map(this->gCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);

	memcpy(dataPtr.pData, &playerMatrices, sizeof(playerMatrices));

	this->gDeviceContext->Unmap(this->gCBuffer, 0);
}
void DX::updateCameraConstantBuffer()
{
	objMatrices cameraMatrices = this->camera->getCameraMatrices();

	D3D11_MAPPED_SUBRESOURCE dataPtr;

	//Låser buffern för GPU:n och hämtar den till CPU
	this->gDeviceContext->Map(this->gCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
	// Copy camera matrix to buffer

	memcpy(dataPtr.pData, &cameraMatrices, sizeof(cameraMatrices));

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

	standardMatrices = { standardMatrices.worldM, standardMatrices.viewM, standardMatrices.projM };

	D3D11_MAPPED_SUBRESOURCE dataPtr;
	//ZeroMemory(&dataPtr, sizeof(D3D11_MAPPED_SUBRESOURCE));

	this->gDeviceContext->Map(this->gCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);

	memcpy(dataPtr.pData, &standardMatrices, sizeof(standardMatrices));

	this->gDeviceContext->Unmap(this->gCBuffer, 0);
}

void DX::flushGame() 
{
	//this->player->flushGame();
}
void DX::startMenuLoop()
{
	this->clearRender();
	this->menuControls();
	this->renderMenu();
	this->gSwapChain->Present(1,0);
}
void DX::menuControls()
{
	this->tButtonPress = GetCurrentTime();

	if (this->isStartMenu == true)
	{
		if (this->tButtonPress - this->lTimePress >= 900)
		{
			if (GetAsyncKeyState(VK_ESCAPE))//Esc
			{
				PostQuitMessage(-1);
			}
		}
	}
	else
	{
		if (GetAsyncKeyState(VK_ESCAPE))//Esc
		{
			if (this->tButtonPress - this->lTimePress >= 900)
			{
				this->flushGame();
				this->menuMsg = false;
				this->isStartMenu = true;
			}
		}
	}


	if (GetAsyncKeyState(0x57)) //w
	{
		if (this->tButtonPress - this->lTimePress >= 300)
		{
			this->lTimePress = GetCurrentTime();

			std::cout << "test" << std::endl;
		}
	}

	if (GetAsyncKeyState(0x53))	//s
	{
		if (this->tButtonPress - this->lTimePress >= 300)
		{
			this->lTimePress = GetCurrentTime();

			std::cout << "test" << std::endl;
		}
	}
	
	if (GetAsyncKeyState(VK_RETURN))
	{
		if (this->tButtonPress - this->lTimePress >= 100)
		{
			this->lTimePress = GetCurrentTime();
			this->flushGame();
			this->menuMsg = false;
			this->isStartMenu = false;
		}
	}
}
void DX::renderMenu()
{
	UINT32 vertexSize = sizeof(float) * 8;
	UINT32 offset = 0;

	this->gDeviceContext->IASetInputLayout(this->gVertexLayout);

	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->gDeviceContext->VSSetShader(this->gVertexShader, nullptr, 0);
	this->gDeviceContext->GSSetShader(this->gGeometryShader, nullptr, 0);
	this->gDeviceContext->PSSetShader(this->gFragmentShader, nullptr, 0);
	this->gDeviceContext->PSSetShaderResources(0, 1, &this->gTextureRTV);
	this->gDeviceContext->PSSetSamplers(0, 1, &this->samplerState);

	this->gDeviceContext->GSSetConstantBuffers(0, 1, &this->menuBuffer);
	this->gDeviceContext->PSSetConstantBuffers(0, 1, &this->shaderBuffer);

	this->gDeviceContext->IASetVertexBuffers(0, 1, &this->gMenuVertexArray, &vertexSize, &offset);
	this->gDeviceContext->Draw(6, 0);

	this->gDeviceContext->IASetVertexBuffers(0, 1, &this->gVertexBufferArray[1], &vertexSize, &offset);
	this->gDeviceContext->Draw(30, 0);

	this->gDeviceContext->IASetVertexBuffers(0, 1, &this->gVertexBufferArray[2], &vertexSize, &offset);
	this->gDeviceContext->Draw(30, 0);
}
void DX::renderInGameMenu()
{
	UINT32 vertexSize = sizeof(float) * 8;
	UINT32 offset = 0;

	this->gDeviceContext->IASetInputLayout(this->gVertexLayout);

	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->gDeviceContext->VSSetShader(this->gVertexShader, nullptr, 0);
	this->gDeviceContext->GSSetShader(this->gGeometryShader, nullptr, 0);
	this->gDeviceContext->PSSetShader(this->gFragmentShader, nullptr, 0);
	this->gDeviceContext->PSSetShaderResources(0, 1, &this->gTextureRTV);
	this->gDeviceContext->PSSetSamplers(0, 1, &this->samplerState);  

	this->gDeviceContext->GSSetConstantBuffers(0, 1, &this->menuBuffer);
	this->gDeviceContext->PSSetConstantBuffers(0, 1, &this->shaderBuffer);

	this->gDeviceContext->IASetVertexBuffers(0, 1, &this->gMenuVertexArray, &vertexSize, &offset);
	this->gDeviceContext->Draw(6, 0);

	this->gDeviceContext->IASetVertexBuffers(0, 1, &this->gVertexBufferArray[3], &vertexSize, &offset);
	this->gDeviceContext->Draw(30, 0);

	this->gDeviceContext->IASetVertexBuffers(0, 1, &this->gVertexBufferArray[4], &vertexSize, &offset);
	this->gDeviceContext->Draw(30, 0);
}

void DX::printMatrices(objMatrices mat)
{
	XMFLOAT4X4 w;
	XMFLOAT4X4 v;
	XMFLOAT4X4 p;

	XMStoreFloat4x4(&w, mat.worldM);
	XMStoreFloat4x4(&v, mat.viewM);
	XMStoreFloat4x4(&p, mat.projM);

	cout << "New World: " << endl;

	cout << w._11 << ", " << w._12 << ", " << w._13 << ", " << w._14 <<  endl;
	cout << w._21 << ", " << w._22 << ", " << w._23 << ", " << w._24 << endl;
	cout << w._31 << ", " << w._32 << ", " << w._33 << ", " << w._34 << endl;
	cout << w._41 << ", " << w._42 << ", " << w._43 << ", " << w._44 << endl;
	cout << "New View: " << endl;
	cout << v._11 << ", " << v._12 << ", " << v._13 << ", " << v._14 << endl;
	cout << v._21 << ", " << v._22 << ", " << v._23 << ", " << v._24 << endl;
	cout << v._31 << ", " << v._32 << ", " << v._33 << ", " << v._34 << endl;
	cout << v._41 << ", " << v._42 << ", " << v._43 << ", " << v._44 << endl;
	cout << "New Proj: " << endl;
	cout << p._11 << ", " << p._12 << ", " << p._13 << ", " << p._14 << endl;
	cout << p._21 << ", " << p._22 << ", " << p._23 << ", " << p._24 << endl;
	cout << p._31 << ", " << p._32 << ", " << p._33 << ", " << p._34 << endl;
	cout << p._41 << ", " << p._42 << ", " << p._43 << ", " << p._44 << endl << endl;;
}
