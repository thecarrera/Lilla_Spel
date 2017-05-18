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
	this->gMenuRTV = new ID3D11ShaderResourceView*[4];

	for (int i = 0; i < 4; i++)
	{
		this->gMenuRTV[i] = nullptr;
	}
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

	SAFE_RELEASE(this->txSamplerState);
	SAFE_RELEASE(this->sMaskSamplerState);
	SAFE_RELEASE(this->ShadowSampler);

	SAFE_DELETE(this->gTextureRTV);

	SAFE_DELETE(this->gVertexBufferArray);
	
}

void DX::OfflineCreation(HMODULE hModule, HWND* wndHandle)
{
	this->CreateDirect3DContext(wndHandle);

	this->SetViewport();

	this->FBX.Import(".\\Assets\\Files\\test.gay", this->gDevice, this->gVertexBufferArray);

	this->gVertexBufferArray_size = FBX.getTotalMeshes();

	this->player = new Player();
	DirectX::XMMATRIX world =
	{	1.0f, 0, 0, 0,
		0, 1.0f, 0, 0,
		0, 0, 1.0f, 0,
		0, 0, 0, 1.f };

	this->createCBuffer(); //kamera

	this->camera = new Camera();

	this->player->initiateMatrices(world, this->camera->getCameraMatrices().viewM, this->camera->getCameraMatrices().projM);

	this->CreateShaders();

	this->createMenu();

	col = Collision(this->FBX.getMeshes(), FBX.getMeshCount());

	interactiveCol = InteractiveCollision(this->FBX.getMeshes(), FBX.getMeshCount());

	this->Texture(this->gDevice, this->gDeviceContext, this->gMenuRTV);
	
	shadowMap->ShadowMapping((int)WIDTH, (int)HEIGHT, this->ShadowmapTex, this->ShadowMask, this->ShadowDepthStencilView, this->ShadowShaderRecourceView, this->gDevice, this->gDeviceContext, this->ShadowMaskResourceView, this->GroundMaskRV);

	createLightCaster();

	this->SM.createFMOD();
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
		1.0f, 1.0f,						//uv
		0.0f, 1.0f, 0.0f,				//Normal

		-40.0f, 0.0f, -32.0f,			//Position
		0.0f, 1.0f,						//uv
		0.0f, 1.0f, 0.0f,				//Normal

		-40.0f, 0.0f, 32.0f,				//Position
		0.0f, 0.0f,						//uv
		0.0f, 1.0f, 0.0f,				//Normal

		40.0f, 0.0f, 32.0f,				//Position
		1.0f, 0.0f,						//uv
		0.0f, 1.0f, 0.0f,				//Normal

		40.0f, 0.0f, -32.0f,			//Position
		1.0f, 1.0f,						//uv
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

	//this->menuMats.worldM *= XMMatrixRotationX(this->degreeToRadians(-90));

	//this->printMatrices(this->menuMats);

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
	SM.update();
	if (this->menuMsg == false)
	{
		clearRender();
		player->move(this->camera, col.calculateCollisionData(player->getMatrices().worldM, this->player->getIsDigging()), this->menuMsg, this->tButtonPress, this->lTimePress, test, this->SM);

		player->move(this->camera, col.calculateCollisionData(player->getMatrices().worldM, player->getIsDigging()), this->menuMsg, this->tButtonPress, this->lTimePress, test, this->SM);

		interactiveCol.test(col.getCollisionData(), col, this->SM);
		
		this->updatePlayerConstantBuffer(); //annars ser inte rör

		//Shadow sampling


		// Character Render pass
		this->Render(0, true); //Debug Comment; RenderPass 1, will take everything and the player to consideration during DepthMap Sampling
		this->Render(1, true);


		//updateraKamera

		this->resetConstantBuffer();
		
		// Enviroment Render pass
		this->Render(0, false);
		this->Render(1, false);

		this->gDeviceContext->ClearDepthStencilView(this->ShadowDepthStencilView, 0x1L, 1, 0);
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
	scd.BufferDesc.Width = (UINT)WIDTH;
	scd.BufferDesc.Height = (UINT)HEIGHT;
	scd.SampleDesc.Count = (int)PIXELSAMPLE;
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
void DX::Render(int pass, bool isPlayer) 
{	

	UINT32 vertexSize = sizeof(float) * 8;
	UINT32 offset = 0;
	if (pass == 0) 
	{
		/**
		Pass 0: Vertex shader exclusive
		**/
		//ShadowMap
		this->gDeviceContext->OMSetRenderTargets(1, &this->gBackBufferRTV, this->ShadowDepthStencilView);
		

		this->gDeviceContext->VSSetShader(this->gShadowVertexShader, nullptr, 0);
		this->gDeviceContext->GSSetShader(nullptr, nullptr, 0);
		this->gDeviceContext->PSSetShader(nullptr, nullptr, 0);

		this->gDeviceContext->IASetInputLayout(this->gShadowLayout);
		this->gDeviceContext->VSSetConstantBuffers(0, 1, &this->lcBuffer);
		this->gDeviceContext->VSSetConstantBuffers(1, 1, &this->gCBuffer);

		this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3D11_MAPPED_SUBRESOURCE dataPtr;
		this->gDeviceContext->Map(this->lcBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
		createLightCaster();

		if (isPlayer)
		{
			lMatrix.worldM = player->getMatrices().worldM;
			//lMatrix.viewM = this->player->getMatrices().viewM;
		}
		else
		{
			lMatrix.worldM = originalLightMatrix.worldM;
		}


		memcpy(dataPtr.pData, &this->lMatrix, sizeof(this->lMatrix));

		this->gDeviceContext->Unmap(this->lcBuffer, 0);
		

		if (isPlayer == true)
		{
			this->gDeviceContext->IASetVertexBuffers(0, 1, &this->gVertexBufferArray[4], &vertexSize, &offset);
			this->gDeviceContext->Draw(FBX.getPlayerSumVertices(), 0);
		}

		if (isPlayer == false)
		{
			for (int i = 5; i < this->gVertexBufferArray_size; i++) {
				if (FBX.getMeshBoundingBox(i) == 0)
				{
					this->gDeviceContext->IASetVertexBuffers(0, 1, &this->gVertexBufferArray[i], &vertexSize, &offset);
					this->gDeviceContext->Draw(FBX.getMeshVertexCount(i), 0);
				}
			}
		}
		

	}
	else if (pass == 1) {
		this->gDeviceContext->OMSetRenderTargets(1, &this->gBackBufferRTV, this->gDSV);
		

		this->gDeviceContext->IASetInputLayout(this->gVertexLayout);

		this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		this->gDeviceContext->VSSetShader(this->gVertexShader, nullptr, 0);
		this->gDeviceContext->GSSetShader(this->gGeometryShader, nullptr, 0);
		this->gDeviceContext->PSSetShader(this->gFragmentShader, nullptr, 0);

		this->gDeviceContext->PSSetShaderResources(0, 1, &this->gMenuRTV[0]);
		this->gDeviceContext->PSSetShaderResources(1, 1, &this->ShadowShaderRecourceView);
		this->gDeviceContext->PSSetShaderResources(2, 1, &this->ShadowMaskResourceView);
		this->gDeviceContext->PSSetShaderResources(3, 1, &this->GroundMaskRV);

		this->gDeviceContext->PSSetSamplers(0, 1, &this->txSamplerState);
		this->gDeviceContext->PSSetSamplers(1, 1, &this->sMaskSamplerState);
		this->gDeviceContext->PSSetSamplers(2, 1, &this->ShadowSampler);

		
		this->gDeviceContext->GSSetConstantBuffers(0, 1, &this->gCBuffer);
		this->gDeviceContext->GSSetConstantBuffers(1, 1, &this->lcBuffer);
		this->gDeviceContext->PSSetConstantBuffers(0, 1, &this->lcBuffer);

		if (isPlayer == true)
		{
			this->gDeviceContext->PSSetShaderResources(0, 1, &this->gMenuRTV[3]);
			this->gDeviceContext->IASetVertexBuffers(0, 1, &this->gVertexBufferArray[4], &vertexSize, &offset);
			this->gDeviceContext->Draw(FBX.getPlayerSumVertices(), 0);
		}

		if (isPlayer == false)
		{
			for (int i = 5; i < this->gVertexBufferArray_size; i++) {
				if (FBX.getMeshBoundingBox(i) == 0)
				{
					this->gDeviceContext->IASetVertexBuffers(0, 1, &this->gVertexBufferArray[i], &vertexSize, &offset);
					this->gDeviceContext->Draw(FBX.getMeshVertexCount(i), 0);
				}
			}
		}
		
	}
}
void DX::clearRender()
{
	float clearColor[] = { 0.0f, 1.0f, 0.0f, 1.f };

	this->gDeviceContext->ClearRenderTargetView(this->gBackBufferRTV, clearColor);
	this->gDeviceContext->ClearDepthStencilView(this->gDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
 
void DX::CreateShaders()
{
	HRESULT hr;

	ID3DBlob* error = nullptr;

	//#############################################################
	//#						Vertex Shader						  #
	//#############################################################
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

	//#############################################################
	//#					Shadow Vertex Shader					  #
	//#############################################################
	ID3DBlob* VSShadow = nullptr;
	D3DCompileFromFile(
		L"VertexShaderShadow.hlsl",
		nullptr,
		nullptr,
		"VS_main",
		"vs_5_0",
		0,
		0,
		&VSShadow,
		&error
	);

	hr = this->gDevice->CreateVertexShader(VSShadow->GetBufferPointer(), VSShadow->GetBufferSize(), nullptr, &this->gShadowVertexShader);

	if (error)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	D3D11_INPUT_ELEMENT_DESC shadowLayout[] =
	{ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };

	this->gDevice->CreateInputLayout(shadowLayout, 1, VSShadow->GetBufferPointer(), VSShadow->GetBufferSize(), &this->gShadowLayout);

	SAFE_RELEASE(VSShadow);
	SAFE_RELEASE(error);

	//Sampler State
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	hr = this->gDevice->CreateSamplerState(&sampDesc, &this->sMaskSamplerState);

	//Shadow Sampler
	D3D11_SAMPLER_DESC shadowSamplerDesc;
	shadowSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	shadowSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	shadowSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	shadowSamplerDesc.MipLODBias = 1;
	shadowSamplerDesc.MaxAnisotropy = 16;


	hr = this->gDevice->CreateSamplerState(&shadowSamplerDesc, &this->ShadowSampler);

	if (FAILED(hr))
	{
		getchar();
	}

	//#############################################################
	//#						Geometry Shader						  #
	//#############################################################
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

	//#############################################################
	//#						Fragment Shader						  #
	//#############################################################
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

	//#############################################################
	//#					Menu Fragment Shader					  #
	//#############################################################
	ID3DBlob* pFS2 = nullptr;
	D3DCompileFromFile(
		L"MenuPixelShader.hlsl",
		nullptr,
		nullptr,
		"FS_main",
		"ps_5_0",
		0,
		0,
		&pFS2,
		&error
	);

	hr = this->gDevice->CreatePixelShader(pFS2->GetBufferPointer(), pFS2->GetBufferSize(), nullptr, &this->gMenuFragmentShader);

	if (error)
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	SAFE_RELEASE(pFS);
	SAFE_RELEASE(error);

	//Sampler State
	D3D11_SAMPLER_DESC sampDesc2;
	ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
	sampDesc2.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc2.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc2.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc2.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc2.MipLODBias = 0;
	sampDesc2.MaxAnisotropy = PIXELSAMPLE;
	sampDesc2.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc2.MinLOD = 0;
	sampDesc2.MaxLOD = 0;

	hr = this->gDevice->CreateSamplerState(&sampDesc2, &this->txSamplerState);


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
	
	D3D11_BUFFER_DESC lcbuff;
	lcbuff.Usage = D3D11_USAGE_DYNAMIC;
	lcbuff.ByteWidth = sizeof(objMatrices);
	lcbuff.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lcbuff.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lcbuff.MiscFlags = 0;
	lcbuff.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA ldat;
	ldat.pSysMem = &this->lMatrix;

	hr = gDevice->CreateBuffer(&lcbuff, &ldat, &this->lcBuffer);


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
void DX::createLightCaster()
{
	//light for Shadowmapping
	DirectX::XMFLOAT4 lightPos = { 30, 50.0f, 2.0f, 1.0f };
	DirectX::XMVECTOR lookTo = { 0.5,-1,0.5 };
	DirectX::XMVECTOR upVec = { 0,1,0 };

	
	DirectX::XMMATRIX worldM = DirectX::XMMatrixIdentity();
	
	player->getPosition(lMatrix.lightPos);
	player->getPositionVec(lookTo);
	
	DirectX::XMMATRIX viewM = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(XMVECTOR{lMatrix.lightPos.x,lightPos.y,lightPos.z,lightPos.w}, lookTo, upVec));
	DirectX::XMMATRIX projM = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(XM_PI * 0.70f, 800.0f/640.0f, 0.1f, 100.0f));

	this->lMatrix.worldM = worldM;
	this->lMatrix.viewM = viewM;
	this->lMatrix.projM = projM;
	this->lMatrix.lightPos = lightPos;

	originalLightMatrix.worldM = lMatrix.worldM;
	originalLightMatrix.viewM = lMatrix.viewM;
	originalLightMatrix.lightPos = lMatrix.lightPos;
	test.viewM = lMatrix.viewM;

	//printMatrices(lMatrix);
}

void DX::flushGame() 
{
	this->createMenu();
	this->camera->flushGame();
	this->player->setMatrices(camera->getCameraMatrices());
	this->player->flushGame();
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
				PostQuitMessage(0);
			}
		}
	}
	else
	{
		if (GetAsyncKeyState(VK_ESCAPE))//Esc
		{
			if (this->tButtonPress - this->lTimePress >= 900)
			{
				this->lTimePress = GetCurrentTime();
				this->flushGame();
				this->menuMsg = false;
				this->isStartMenu = true;
				this->SM.stopSound(0);
				this->SM.stopSound(1);
				this->SM.stopSound(2);
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
	if (this->isStartMenu == true) 
	{
		if (this->tButtonPress - this->lTimePress >= 900)  
		{
			if (GetAsyncKeyState(VK_RETURN))//Esc
			{
				this->lTimePress = GetCurrentTime();
				this->menuMsg = false;
				this->isStartMenu = false;
				this->SM.playSound(0);
				this->SM.setVolume(0, 0.4f);
			}
		}
	}
	else
	{
		if (this->tButtonPress - this->lTimePress >= 900)
		{
			if (GetAsyncKeyState(VK_RETURN))//Esc
			{
				this->lTimePress = GetCurrentTime();
				this->menuMsg = false;
				this->isStartMenu = false;
				this->SM.togglePauseSound(0, false);
				this->SM.togglePauseSound(1, false);
				this->SM.togglePauseSound(2, false);
			}
		}
	}
}
void DX::renderMenu()
{
	UINT32 vertexSize = sizeof(float) * 8;
	UINT32 offset = 0;

	this->gDeviceContext->OMSetRenderTargets(1, &this->gBackBufferRTV, this->gDSV);
	this->gDeviceContext->IASetInputLayout(this->gVertexLayout);

	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->gDeviceContext->VSSetShader(this->gVertexShader, nullptr, 0);
	this->gDeviceContext->GSSetShader(this->gGeometryShader, nullptr, 0);
	this->gDeviceContext->PSSetShader(this->gMenuFragmentShader, nullptr, 0);
	this->gDeviceContext->PSSetShaderResources(0, 1, &this->gMenuRTV[0]);
	this->gDeviceContext->PSSetSamplers(0, 1, &this->txSamplerState);

	this->gDeviceContext->GSSetConstantBuffers(0, 1, &this->menuBuffer);

	this->gDeviceContext->IASetVertexBuffers(0, 1, &this->gMenuVertexArray, &vertexSize, &offset);
	this->gDeviceContext->Draw(6, 0);

	this->gDeviceContext->PSSetShaderResources(0, 1, &this->gMenuRTV[1]);

	this->gDeviceContext->IASetVertexBuffers(0, 1, &this->gVertexBufferArray[0], &vertexSize, &offset);
	this->gDeviceContext->Draw(36, 0);

	this->gDeviceContext->IASetVertexBuffers(0, 1, &this->gVertexBufferArray[1], &vertexSize, &offset);
	this->gDeviceContext->Draw(36, 0);
}
void DX::renderInGameMenu()
{
	UINT32 vertexSize = sizeof(float) * 8;
	UINT32 offset = 0;

	this->gDeviceContext->OMSetRenderTargets(1, &this->gBackBufferRTV, this->gDSV);
	this->gDeviceContext->IASetInputLayout(this->gVertexLayout);

	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->gDeviceContext->VSSetShader(this->gVertexShader, nullptr, 0);
	this->gDeviceContext->GSSetShader(this->gGeometryShader, nullptr, 0);
	this->gDeviceContext->PSSetShader(this->gMenuFragmentShader, nullptr, 0);
	this->gDeviceContext->PSSetShaderResources(0, 1, &this->gMenuRTV[0]);
	this->gDeviceContext->PSSetSamplers(0, 1, &this->txSamplerState);  

	this->gDeviceContext->GSSetConstantBuffers(0, 1, &this->menuBuffer);

	this->gDeviceContext->IASetVertexBuffers(0, 1, &this->gMenuVertexArray, &vertexSize, &offset);
	this->gDeviceContext->Draw(6, 0);

	this->gDeviceContext->PSSetShaderResources(0, 1, &this->gMenuRTV[2]);

	this->gDeviceContext->IASetVertexBuffers(0, 1, &this->gVertexBufferArray[2], &vertexSize, &offset);
	this->gDeviceContext->Draw(36, 0);

	this->gDeviceContext->IASetVertexBuffers(0, 1, &this->gVertexBufferArray[3], &vertexSize, &offset);
	this->gDeviceContext->Draw(36, 0);
}
void DX::Texture(ID3D11Device* &gDevice, ID3D11DeviceContext* &gDeviceContext, ID3D11ShaderResourceView** &RTV)
{
	HRESULT hr;

	wchar_t fileName[256];
	char temp1[256] = ".\\Assets\\Textures\\Menu\\Menu_BG_art2.jpg";
	char temp2[256] = ".\\Assets\\Textures\\Menu\\Buttons_startMenu.jpg";
	char temp3[256] = ".\\Assets\\Textures\\Menu\\Buttons_Pause_BG.jpg";
	char temp4[256] = ".\\Assets\\Textures\\Models\\Merlin_Lowpoly.png";

	for (int i = 0; i < sizeof(temp1); i++)
	{
		fileName[i] = temp1[i];
	}

	hr = DirectX::CreateWICTextureFromFile(gDevice, gDeviceContext, fileName, NULL, &RTV[0], NULL);

	if (FAILED(hr))
	{
		return exit(-1);
	}

	for (int i = 0; i < sizeof(temp2); i++)
	{
		fileName[i] = temp2[i];
	}

	hr = DirectX::CreateWICTextureFromFile(gDevice, gDeviceContext, fileName, NULL, &RTV[1], NULL);

	if (FAILED(hr))
	{
		return exit(-1);
	}

	for (int i = 0; i < sizeof(temp3); i++)
	{
		fileName[i] = temp3[i];
	}

	hr = DirectX::CreateWICTextureFromFile(gDevice, gDeviceContext, fileName, NULL, &RTV[2], NULL);

	if (FAILED(hr))
	{
		return exit(-1);
	}

	for (int i = 0; i < sizeof(temp4); i++)
	{
		fileName[i] = temp4[i];
	}

	hr = DirectX::CreateWICTextureFromFile(gDevice, gDeviceContext, fileName, NULL, &RTV[3], NULL);

	if (FAILED(hr))
	{
		return exit(-1);
	}

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
