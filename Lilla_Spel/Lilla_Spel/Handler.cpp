#include "Handler.h"

Handler::Handler()
{
	this->nrOfParticles = 0;
	this->maxParticles = 17000;

	for (int i = 0; i < this->maxParticles; i++)
	{
		this->particles[i] = nullptr;

		//this->particleArray[i] = hlslParticle(DirectX::XMFLOAT3(0, 0, 0), 0, DirectX::XMFLOAT4(0, 0, 0, 0));
	}

	srand(static_cast <unsigned> (time(0)));
	
	//Dive?
	this->now = GetCurrentTime();

	//////time
	this->spawnTimer = 0;
	this->spawnTimer2 = 0;
	this->spawnTimer3 = 0;
	this->spawnTimer4 = 0;
	this->spawnTimer5 = 0;
	this->spawnTimer6 = 0;
	this->spawnTimer7 = 0;
	this->spawnTimer8 = 0;
	this->frameTime = 0;
	this->spawnDelay = 0.00000035f;	//ground 
	this->spawnDelay2 = 0.00000025f;	//leafes
	this->spawnDelay3 = 0.00000025f;	//sun
	this->spawnDelay4 = 0.00000025f; //water
	this->spawnDelay5 = 0.00000025f;	//fire
	this->spawnDelay6 = 0.00005f;	//löv träd
	this->spawnDelay7 = 0.00000025f;	//dig Splash
	this->spawnDelay8 = 0.00000025f;	//Diamanter

	//partikel plats

	this->centrePoint2 = { -1.0f, 0.5f, -1 };
	this->radiusRandomizer2 = { 0.4f, 0.4f, 0.4f };	//leaves

	this->centrePoint3 = { 1.2f, 1.2f, 1 };			//sun
	this->radiusRandomizer3 = { 0, 0, 0 };

	//för worldMatris snurren
	this->snurr = 0;

	//sun
	this->angle = 0;
	this->angle2 = 180;
	this->angle3 = 0;
	this->angle3 = 0;

	this->color = { 0, 0, 0 };
	this->color2 = { 0, 0, 0 };

	//fire
	this->left = true;

	//respawner
	this->emitParticles = true;

	//water
	this->startWaterfallSplash == false;
}

Handler::~Handler()
{
	for (int i = 0; i < this->nrOfParticles; i++)
	{
		delete this->particles[i];
	}

	gVertexBuffer1->Release();
	//GSCONSTANTBUFFER1->Release();

	gVertexLayout1->Release();
	gVertexShader1->Release();
	gGeometryShader1->Release();
	gPixelShader1->Release();
}

void Handler::createVertexBuffer(ID3D11Device* gDevice)
{
	//createVertexBuffer
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc)); //zeroar memoriet
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.ByteWidth = sizeof(this->particleArray);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &this->particleArray;
	HRESULT hr = gDevice->CreateBuffer(&bufferDesc, &data, &gVertexBuffer1);
	if (FAILED(hr))
		exit(-2);
}

void Handler::updateVertexBuffer(ID3D11DeviceContext * gDeviceContext)
{
	for (int i = 0; i < this->nrOfParticles; i++)
	{
		this->particleArray[i] = this->particles[i]->getParticle();
	}

	D3D11_MAPPED_SUBRESOURCE pData;

	//update constantbuffer
	gDeviceContext->Map(this->gVertexBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData); //GPU kan inte ändra

	memcpy(pData.pData, &particleArray, sizeof(hlslParticle) * this->nrOfParticles); // subresourcens data blir matriserna och subrecoursen är bindad till constantbuffern så ändrar vi den ändras constantbuffern

	gDeviceContext->Unmap(this->gVertexBuffer1, 0); //GPU kan nu ändra igwen
}

void Handler::CreateShaders(ID3D11Device* gDevice)
{
	//create vertex shader
	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(
		L"VertexShaderParticle.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"main",		// entry point
		"vs_5_0",		// shader model (target)
		0,				// shader compile options			// here DEBUGGING OPTIONS
		0,				// effect compile options
		&pVS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &gVertexShader1);

	//create input layout (verified using vertex shader)
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "WHATTRIANGLE", 0, DXGI_FORMAT_R32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "PADDING", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS->GetBufferPointer(), pVS->GetBufferSize(), &gVertexLayout1);
	// we do not need anymore this COM object, so we release it.
	pVS->Release();

	//create geometry shader
	ID3DBlob* pGS = nullptr;
	D3DCompileFromFile(
		L"GeometryShaderParticle.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"main",		// entry point
		"gs_5_0",		// shader model (target)
		0,				// shader compile options			// here DEBUGGING OPTIONS
		0,				// effect compile options
		&pGS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	gDevice->CreateGeometryShader(pGS->GetBufferPointer(), pGS->GetBufferSize(), nullptr, &gGeometryShader1);
	// we do not need anymore this COM object, so we release it.
	pGS->Release();

	//create pixel shader
	ID3DBlob* pPS = nullptr;
	D3DCompileFromFile(
		L"PixelShaderParticle.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"main",		// entry point
		"ps_5_0",		// shader model (target)
		0,				// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &gPixelShader1);
	// we do not need anymore this COM object, so we release it.
	pPS->Release();
}

D3D11_SUBRESOURCE_DATA Handler::initiateSubresources()
{
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &this->matriser;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;
	return data;
}

void Handler::initiateMatrices()
{
	//Detta initiatear fel, update fixar detta

	float FovAngleY = 3.14 * 0.45;
	float AspectRatio = 640 / 480;
	int zFar = 20;
	double zNear = 0.1;

	//Rotate matrix around Y Axis //initiate så standard för nu
	matriser.world = DirectX::XMMATRIX(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	DirectX::XMVECTOR position = DirectX::XMVectorSet(0, 0, -1, 0);
	DirectX::XMVECTOR look = DirectX::XMVectorSet(0, 0, 1, 0);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0, 1, 0, 0);

	matriser.view = DirectX::XMMatrixLookAtLH(position, look, up);
	matriser.view = DirectX::XMMatrixTranspose(matriser.view);	// transponering -> rader = kolloner och tvärt om

	matriser.projection = DirectX::XMMatrixPerspectiveFovLH(FovAngleY, AspectRatio, zNear, zFar);
	matriser.projection = DirectX::XMMatrixTranspose(matriser.projection);

}

void Handler::updateMatrices(ID3D11DeviceContext* gDeviceContext, objMatrices* cameraMatrices)
{
	matriser.view = cameraMatrices->viewM;
	matriser.projection = cameraMatrices->projM;

	this->GSCONSTANTBUFFER1;

	D3D11_MAPPED_SUBRESOURCE pData;

	//update constantbuffer
	gDeviceContext->Map(GSCONSTANTBUFFER1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData); //GPU kan inte ändra

	memcpy(pData.pData, &matriser, sizeof(matrixStruct)); // subresourcens data blir matriserna och subrecoursen är bindad till constantbuffern så ändrar vi den ändras constantbuffern

	gDeviceContext->Unmap(GSCONSTANTBUFFER1, 0); //GPU kan nu ändra igwen


	////Rotate matrix around Y Axis
	//matriser.world = DirectX::XMMATRIX(
	//	cos(snurr), 0, -sin(snurr), 0,
	//	0, 1, 0, 0,
	//	sin(snurr), 0, cos(snurr), 0,
	//	0, 0, 0, 1);

	//snurr += 0.1;

	//matriser.world *= DirectX::XMMatrixRotationRollPitchYaw(0, 0.01, 0); //snurr
}




ID3D11Buffer* Handler::createConstantBuffer(UINT size, bool dynamic, bool CPUupdates, D3D11_SUBRESOURCE_DATA* pData, ID3D11Device* gDevice) //pData = all data
{
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = size;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// Select the appropriate usage and CPU access flags based on the passed flags, vi kommer sätta true på CPUupdates och dynamic
	if (dynamic && CPUupdates)
	{
		desc.Usage = D3D11_USAGE_DYNAMIC;	//att GPU kan läsa och CPU skriva, map metod krävs vid ändring av data
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	// Create the buffer with the specified configuration
	ID3D11Buffer* pBuffer = 0;
	HRESULT hr = gDevice->CreateBuffer(&desc, pData, &pBuffer);
	if (FAILED(hr))
		exit(-1);
	return(pBuffer);
}

	
void Handler::CreateTriangleData(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext,
	int collisionType, Player* player)
{
	//time
	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	elapsed_seconds.count(); //actual time

	this->frameTime = elapsed_seconds.count();
	this->spawnTimer += elapsed_seconds.count();
	this->spawnTimer2 += elapsed_seconds.count();
	this->spawnTimer3 += elapsed_seconds.count();
	this->spawnTimer4 += elapsed_seconds.count();
	this->spawnTimer5 += elapsed_seconds.count();
	this->spawnTimer6 += elapsed_seconds.count();
	this->spawnTimer7 += elapsed_seconds.count();
	this->spawnTimer8 += elapsed_seconds.count();
	
	player->getDirection(); //DENNA

	//Dig
	this->now = GetCurrentTime();
	time_t timePlayer = player->getTimeWhenBurrowed();

	if (this->emitParticles)
	{
		//create ground
		if (this->nrOfParticles < this->maxParticles - 1 && this->spawnTimer > this->spawnDelay) //används flera emmiters för att inte få värden nära 0 på randomizers
		{
			if (this->lastPlaterPos.x != player->getPlayerPos().x || this->lastPlaterPos.y != player->getPlayerPos().y || this->lastPlaterPos.z != player->getPlayerPos().z)
			{
				DirectX::XMFLOAT3 particlePos = player->getPlayerPos();
				particlePos.y = 0.3f;

				DirectX::XMFLOAT3 color = { 139, 69, 19 };

				if (player->getFlyingUp() == true)
				{
					bool boll = 0; //Fixa ingen mark när man hoppar
				}

				if (player->getPlayerPos().x > 1130 && player->getPlayerPos().x < 1200)
				{
					//grått
					color = { 90, 77, 65 };
				}

				if (player->getPlayerPos().x > 1500 && player->getPlayerPos().x < 1840)
				{
					//blått
					color = { 2, 147, 101 };
				}

				if (collisionType == -1 && player->getFlyingUp() == false)
				{
					//under fötterna
					Particle* newParticle = new Particle();
					newParticle->createParticle(particlePos, { 0.8f, 0, 0.8f }, { 0, 0, 0 }, { -1, -1, -1 }, { 1, 1, 1 }, color, 10, 15, 0.000015f, { 0.05f, 0.05f, 0.05f }, 0, 100000, 0);

					this->particles[this->nrOfParticles] = newParticle;
					this->nrOfParticles++;

					this->spawnTimer = 0;
				}
			}
		}
		//dig splash
		if (this->now >= timePlayer + 500 && this->now <= timePlayer + 600)
		{
			if (player->getIsDigging() == true)
			{
				if (this->nrOfParticles < this->maxParticles - 20 && this->spawnTimer7 > this->spawnDelay7)
				{
					DirectX::XMFLOAT3 color = { 139, 69, 19 };

					if (player->getPlayerPos().x > 1130 && player->getPlayerPos().x < 1200)
					{
						//grått
						color = { 90, 77, 65 };
					}

					for (int i = 0; i < 20; i++)
					{
						Particle* newParticle = new Particle();
						newParticle->createParticle(player->getPlayerPos(), { 1, 1, 1 }, { 0, 0, 0 }, { -1, 0, -1 }, { 1, 1, 1 }, color, 10, 15, 0.00003f, { 0.1f, 0.1f, 0.1f }, 0, 100000, 0);

						this->particles[this->nrOfParticles] = newParticle;
						this->nrOfParticles++;

						this->spawnTimer7 = 0;
					}
				}
			}
		}

		//emersion splash
		if (this->now >= timePlayer + 100 && this->now <= timePlayer + 200)
		{
			if (player->getIsDigging() == false)
			{
				if (this->nrOfParticles < this->maxParticles - 20 && this->spawnTimer7 > this->spawnDelay7)
				{
					DirectX::XMFLOAT3 color = { 139, 69, 19 };

					if (player->getPlayerPos().x > 1130 && player->getPlayerPos().x < 1200)
					{
						//grått
						color = { 90, 77, 65 };
					}

					for (int i = 0; i < 20; i++)
					{
						Particle* newParticle = new Particle();
						newParticle->createParticle(player->getPlayerPos(), { 1, 1, 1 }, { 0, 0, 0 }, { -1, 0, -1 }, { 1, 1, 1 }, color, 10, 15, 0.00003f, { 0.1f, 0.1f, 0.1f }, 0, 100000, 0);

						this->particles[this->nrOfParticles] = newParticle;
						this->nrOfParticles++;

						this->spawnTimer7 = 0;
					}
				}
			}
		}

		//leaves for bush
		if (this->nrOfParticles < this->maxParticles -1 && this->spawnTimer2 > this->spawnDelay2)
		{
			if (this->lastPlaterPos.x != player->getPlayerPos().x || this->lastPlaterPos.y != player->getPlayerPos().y || this->lastPlaterPos.z != player->getPlayerPos().z)
			{
				if (player->getPlayerPos().x >= 174 && player->getPlayerPos().x <= 178)
				{
					if (player->getPlayerPos().z >= -16 && player->getPlayerPos().z <= 1)
					{
						DirectX::XMFLOAT3 directionVector = { 0, 0, 0 };
						DirectX::XMFLOAT3 directionRand = { 0, 0, 0 };

						if (player->getDirection() == "w")
						{
							directionVector = { 0, 0, -1 };
							directionRand = { 1, 0, 0 };
						}
						else if (player->getDirection() == "a")
						{
							directionVector = { 1, 0, 0 };
							directionRand = { 0, 0, 1 };
						}
						else if (player->getDirection() == "s")
						{
							directionVector = { 0, 0, 1 };
							directionRand = { 1, 0, 0 };
						}
						else if (player->getDirection() == "d")
						{
							directionVector = { -1, 0, 0 };
							directionRand = { 0, 0, 1 };
						}

						Particle* newParticle = new Particle();
						newParticle->createParticle({ player->getPlayerPos().x, 0.3f, player->getPlayerPos().z }, { 0.2f, 0, 0.2f }, directionVector, { -directionRand.x, -directionRand.y, -directionRand.z }, directionRand, { 0, 255, 0 }, 15, 20, 0.000015f, { 0.05f, 0.05f, 0.05f }, 0, 100000, 0); 

						this->particles[this->nrOfParticles] = newParticle;
						this->nrOfParticles++;

						this->spawnTimer2 = 0;
					}
				}
			}
		}
		//löv för träd
		//286, 0, 54
		if (player->getPlayerPos().x <= 370)
		{
			if (this->nrOfParticles < this->maxParticles - 4 && this->spawnTimer6 > this->spawnDelay6)
			{
				//tree 2
				Particle* newParticle = new Particle();
				newParticle->createParticle({ 90, 12.5f, 0 }, { 8, 0, 8 }, { 0, -1, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 34, 139, 34 }, 10, 15, 1, { 0, 0.01f, 0 }, 0, 100, 0); 

				this->particles[this->nrOfParticles] = newParticle;
				this->nrOfParticles++;

				//tree 1
				Particle* newParticle2 = new Particle();
				newParticle2->createParticle({ 0, 12.5f, 14.3f }, { 8, 0, 8 }, { 0, -1, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 34, 139, 34 }, 10, 15, 1, { 0, 0.01f, 0 }, 0, 100, 0); 

				this->particles[this->nrOfParticles] = newParticle2;
				this->nrOfParticles++;

				//tree waterfall
				Particle* newParticle3 = new Particle();
				newParticle3->createParticle({ 217, 12.5f, 41 }, { 8.5f, 0, 8.5f }, { 0, -1, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 34, 139, 34 }, 10, 15, 1, { 0, 0.01f, 0 }, 0, 100, 0); 

				this->particles[this->nrOfParticles] = newParticle3;
				this->nrOfParticles++;

				//tree waterfall2
				Particle* newParticle4 = new Particle();
				newParticle4->createParticle({ 286, 12.5f, 54 }, { 8.5f, 0, 8.5f }, { 0, -1, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 34, 139, 34 }, 10, 15, 1, { 0, 0.01f, 0 }, 0, 100, 0); 

				this->particles[this->nrOfParticles] = newParticle4;
				this->nrOfParticles++;

				this->spawnTimer6 = 0;
			}
		}
		
		//Waterfall splash
		if (player->getPlayerPos().x >= 30 && player->getPlayerPos().x <= 370)
		{
			if (this->startWaterfallSplash == true)
			{
				if (this->nrOfParticles < this->maxParticles - 15 && this->spawnTimer4 > this->spawnDelay4)
				{
					for (int i = 0; i < 15; i++)
					{
						Particle* newParticle = new Particle();
						newParticle->createParticle({ 248, 0, 79 }, { 0, 0, 10 }, { 1, 0, 0 }, { 0.2f, 0, 0 }, { 0.5f, 0, 0 }, { 0, 255, 255 }, 40, 50, 0.0001f, { 0.15f, 0, 0 }, 0, 1000000, 2); 

						this->particles[this->nrOfParticles] = newParticle;
						this->nrOfParticles++;
					}
					this->spawnTimer4 = 0;
				}
			}
		}

		//Waterflow
		if (player->getPlayerPos().x >= 30 && player->getPlayerPos().x <= 370)
		{
			if (this->nrOfParticles < this->maxParticles - 30 && this->spawnTimer5 > this->spawnDelay5)
			{
				//waterflowRight
				for (int i = 0; i < 15; i++)
				{
					Particle* newParticle = new Particle();
					newParticle->createParticle({ 160, 0.5f, 79 }, { 1, 0, 10 }, { 3, 0, 0 }, { 0.5f, 0, 0 }, { 2, 0, 0 }, { 0, 255, 255 }, 40, 50, 1, { 0.05f, 0, 0 }, 0, 0, 1); 

					this->particles[this->nrOfParticles] = newParticle;
					this->nrOfParticles++;
				}
				//waterflowUpp
				for (int i = 0; i < 15; i++)
				{
					Particle* newParticle = new Particle();
					newParticle->createParticle({ 170, 0.5f, 23 }, { 10, 0, 1 }, { 0, 0, 3 }, { 0, 0, 0.5f }, { 0, 0, 2 }, { 0, 255, 255 }, 40, 50, 1, { 0, 0, 0.05f }, 0, 0, 3); 

					this->particles[this->nrOfParticles] = newParticle;
					this->nrOfParticles++;
				}

				this->spawnTimer5 = 0;
			}
		}
		
		//Diamanter
		/*if (player->getPlayerPos().x >= 800 && player->getPlayerPos().x <= 1200)
		{*/
			if (this->nrOfParticles < this->maxParticles - 1 && this->spawnTimer8 > this->spawnDelay8)
			{
				Particle* newParticle = new Particle();
				newParticle->createParticle({ 1014, 26, 62 }, { 21.5f, 0, 0 }, { 0, -1, -1 }, { 0, 0, 0 }, { 0, 0, 0 }, { 138, 43, 226 }, 10, 20, 0.0005f, { 0.05f, 0.05f, 0.01f }, 0, 0, 0);

				this->particles[this->nrOfParticles] = newParticle;
				this->nrOfParticles++;

				this->spawnTimer8 = 0;
			}
		//}

	}

	if (this->nrOfParticles == this->maxParticles)
	{
		this->emitParticles = false;
	}
	if (this->nrOfParticles == 0)
	{
		this->emitParticles = true;
	}

	//checka för deletion
	for (int i = 0; i < this->nrOfParticles; i++)
	{
		if (this->particles[i]->getAge() > this->particles[i]->getLifetime() || this->particles[i]->getParticle().position.y < 0 || this->particles[i]->getID() == 1 || this->particles[i]->getID() == 2 || this->particles[i]->getID() == 3)
		{
			//waterflowRight
			if (this->particles[i]->getID() == 1)
			{
				if (this->particles[i]->getParticle().position.x >= 248) //waterflow
				{
					this->startWaterfallSplash = true;

					Particle* temp = this->particles[this->nrOfParticles - 1];
					this->particles[this->nrOfParticles - 1] = nullptr;
					this->particles[i] = temp;
 					this->nrOfParticles--;
					i--; //checka om den ska tas bort också.
				}
			}
			//waterflowUpp
			else if (this->particles[i]->getID() == 3)
			{
				if (this->particles[i]->getParticle().position.z >= 86) //waterflow
				{
					Particle* temp = this->particles[this->nrOfParticles - 1];
					this->particles[this->nrOfParticles - 1] = nullptr;
					this->particles[i] = temp;
					this->nrOfParticles--;
					i--; //checka om den ska tas bort också.
				}
			}
			else if (this->particles[i]->getID() == 2) //waterfall
			{
				if (this->particles[i]->getParticle().position.y <= -35)
				{
					Particle* temp = this->particles[this->nrOfParticles - 1];
					this->particles[this->nrOfParticles - 1] = nullptr;
					this->particles[i] = temp;
					this->nrOfParticles--;
					i--; //checha om den ska tas bort också.
				}
			}
			else	//regulars
			{
				Particle* temp = this->particles[this->nrOfParticles - 1];
				this->particles[this->nrOfParticles - 1] = nullptr;
				this->particles[i] = temp;
				this->nrOfParticles--;
				i--; //checha om den ska tas bort också.
			}
		}
	}
	//uppdate
	for (int i = 0; i < this->nrOfParticles; i++)
	{
		this->particles[i]->updateParticle(this->frameTime);
	}

	this->updateVertexBuffer(gDeviceContext);

	//player
	this->lastPlaterPos = player->getPlayerPos();
}

	
void Handler::Render(ID3D11DeviceContext* gDeviceContext)
{
	gDeviceContext->VSSetShader(gVertexShader1, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(gGeometryShader1, nullptr, 0);	//visar det på skärmen
	gDeviceContext->PSSetShader(gPixelShader1, nullptr, 0);

	UINT32 vertexSize = sizeof(float) * 12; //MÅSTE ÄNDRAS NÄR COLOR OCH SÅNT LÄGGGS IN!!!!!!!!!!
	UINT32 offset = 0;
	gDeviceContext->IASetVertexBuffers(0, 1, &gVertexBuffer1, &vertexSize, &offset);

	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	gDeviceContext->IASetInputLayout(gVertexLayout1);

	gDeviceContext->GSSetConstantBuffers(0, 1, &GSCONSTANTBUFFER1); //0 för b0 föra att använda rätt buffer

	gDeviceContext->Draw(this->nrOfParticles, 0); //antal partiklar
}

void Handler::initiateThatThing(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext)
{
	this->initiateMatrices(); //inga nya matriser
	D3D11_SUBRESOURCE_DATA data = this->initiateSubresources(); //HUR FÅR JAG TAG I SUBRESOURCE DATAN? ^
	GSCONSTANTBUFFER1 = this->createConstantBuffer(sizeof(matrixStruct), true, true, &data, gDevice);
	gDeviceContext->GSSetConstantBuffers(0, 1, &GSCONSTANTBUFFER1);
	this->createVertexBuffer(gDevice);
}


//löv långsammare, färre, löv bra nu?





//Waterfall
//newParticle->createParticle({ 150, 0, 30 }, { 0, 0, 0 }, { 0, 1, 0 }, { -1, 0, -1 }, { 1, 0, 1 }, { 64, 164, 223 }, 20, 30, 0.5f, { 0.01f, 0.091f, 0.01f }, 0, 10000);

////fire tourch
//if (this->nrOfParticles < this->maxParticles-3 && this->spawnTimer5 > this->spawnDelay5)
//{
//	if (this->left)
//	{
//		this->angle3 += 2;
//		if (this->angle3 >= 180)
//			this->left = false;
//	}

//	if (this->left == false)
//	{
//		this->angle3 -= 2;
//		if (this->angle3 <= 0)
//			this->left = true;
//	}

//	float totAngle = this->angle3 * (3.1415 / 180); //conver to radians

//	float x = cos(totAngle);
//	float y = sin(totAngle);

//	Particle* newParticle = new Particle();
//	newParticle->createParticle({ 29.5f, 9, 12 }, { 0.03f, 0, 0 }, { x, y, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 256, 12, 0 }, 25, 25, 0.0001f, { 0.0006f, 0.07f, 0 }, 0.7f, 0);

//	this->particles[this->nrOfParticles] = newParticle;
//	this->nrOfParticles++;

//	Particle* newParticle2 = new Particle();
//	newParticle2->createParticle({ 29.5f, 9, 12 }, { 0.03f, 0, 0 }, { 0, 1, 0 }, { -30, -1, 0 }, { 30, 1, 0 }, { 256, 12, 0 }, 12.5f, 30, 0.001f, { 0.00001f, 0.005f, 0 }, 0.7f, 0);
//
//	this->particles[this->nrOfParticles] = newParticle2;
//	this->nrOfParticles++;

//	if (this->forward)
//	{
//		this->angle4 += 2;
//		if (this->angle4 >= 180)
//			this->forward = false;
//	}

//	if (this->forward == false)
//	{
//		this->angle4 -= 2;
//		if (this->angle4 <= 0)
//			this->forward = true;
//	}

//	float totAngle2 = this->angle4 * (3.1415 / 180); //conver to radians
//	
//	float y2 = sin(totAngle2);
//	float z2 = tan(totAngle2);		//z-led KANSKE INTE ÄR TAN!!

//	Particle* newParticle3 = new Particle();
//	newParticle3->createParticle({ 29.5f, 9, 12 }, { 0, 0, 0.03f }, {0, y2, z2 }, { 0, 0, 0 }, { 0, 0, 0 }, { 256, 12, 0 }, 25, 25, 0.0001f, { 0, 0.07f, 0.0006f }, 0.7f, 0);
//	
//	this->particles[this->nrOfParticles] = newParticle3;
//	this->nrOfParticles++;

//	Particle* newParticle4 = new Particle();
//	newParticle4->createParticle({ 29.5f, 9, 12 }, { 0, 0, 0.03f }, { 0, 1, 0 }, { 0, -1, -30 }, { 0, 1, 30 }, { 256, 12, 0 }, 12.5f, 30, 0.001f, { 0, 0.005f, 0.00001f }, 0.7f, 0);

//	this->particles[this->nrOfParticles] = newParticle4;
//	this->nrOfParticles++;

//	float angle22 = 90 * (3.1415 / 180);
//	float x22 = cos(angle22);
//	float y22 = sin(angle22);
//	
//	this->spawnTimer5 = 0;
//}

////create leaves
//if (this->nrOfParticles < this->maxParticles && this->spawnTimer2 > this->spawnDelay2)
//{
//	Particle* newParticle = new Particle();
//	newParticle->createParticle(this->centrePoint2, this->radiusRandomizer2, { 0, 0, 0 }, { -0.01f, -0.01f, 0 }, { 0.01f, 0.01f, 0 }, { 58, 95, 11 }, 0.5f, 0.5f, 0.008, { 0.01f, 0.005f, 0.005f }, 0.2f, 0); //5 gravity, size 3

//	this->particles[this->nrOfParticles] = newParticle;
//	this->nrOfParticles++;
//	this->spawnTimer2 = 0;
//}

////create sun
//if (this->nrOfParticles < this->maxParticles - 1 && this->spawnTimer3 > this->spawnDelay3)
//{
//	this->color = { 256, 200, 0 };
//	this->color2 = { 256, 200, 0 };

//	this->angle -= 2;
//	this->angle2 -= 2;

//	float totAngle = this->angle * (3.1415 / 180); //conver to radians
//	float totAngle2 = this->angle2 * (3.1415 / 180); //conver to radians

//	float x = cos(totAngle);
//	float y = sin(totAngle);

//	float x2 = cos(totAngle2);
//	float y2 = sin(totAngle2);

//	Particle* newParticle = new Particle();
//	newParticle->createParticle(this->centrePoint3, this->radiusRandomizer3, { x, y, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { color }, 0.5f, 0.5f, 0.002f, { 0.00007f, 0.00007f, 0 }, 0, 0);
//	this->particles[this->nrOfParticles] = newParticle;
//	this->nrOfParticles++;

//	Particle* newParticle2 = new Particle();
//	newParticle2->createParticle(this->centrePoint3, this->radiusRandomizer3, { x2, y2, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { color2 }, 0.5f, 0.5f, 0.002f, { 0.00007f, 0.00007f, 0 }, 0, 0);
//	this->particles[this->nrOfParticles] = newParticle2;
//	this->nrOfParticles++;
//	this->spawnTimer3 = 0;
//}