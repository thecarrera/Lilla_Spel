#ifndef HANDLER_H
#define HANDLER_H

#include "Particle.h"
#include "Player.h"

#include <chrono> //istället för <ctime> 

class Handler
{
private:
	struct matrixStruct
	{
		//matriserna
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};
	matrixStruct matriser;

	//Dive?
	time_t now;

	//sun
	int direction;
	float angle;
	float angle2;
	DirectX::XMFLOAT3 color;
	DirectX::XMFLOAT3 color2;

	//fire
	float angle3;
	float angle4;

	bool left;
	bool forward;

	//respawner
	bool emitParticles;

	//spawn waterfall
	bool startWaterfallSplash;

	//snurr
	float snurr;
	float trans = 0;

	//partikel plats
	DirectX::XMFLOAT3 centrePoint2;
	DirectX::XMFLOAT3 radiusRandomizer2;

	DirectX::XMFLOAT3 centrePoint3;
	DirectX::XMFLOAT3 radiusRandomizer3;

	//time
	float frameTime;

	float spawnTimer;
	float spawnDelay;

	float spawnTimer2;
	float spawnDelay2;

	float spawnTimer3;
	float spawnDelay3;

	float spawnTimer4;
	float spawnDelay4;

	float spawnTimer5;
	float spawnDelay5;

	float spawnTimer6;
	float spawnDelay6;

	float spawnTimer7;
	float spawnDelay7;

	std::random_device rd;
	Particle partikel;

	hlslParticle particleArray[18000]; //18250 är nära krash, allt kan krasha
	Particle* particles[18000];
	int nrOfParticles;
	int maxParticles;

	ID3D11Buffer* gVertexBuffer1 = nullptr;

	ID3D11InputLayout* gVertexLayout1 = nullptr;
	ID3D11VertexShader* gVertexShader1 = nullptr;
	ID3D11GeometryShader* gGeometryShader1 = nullptr; //GS GÖR FÖRSTA
	ID3D11PixelShader* gPixelShader1 = nullptr;

	ID3D11Buffer* GSCONSTANTBUFFER1 = nullptr;

	DirectX::XMFLOAT3 lastPlaterPos;

	//DepthStencilView kanske behövsi stora eller finns redan?

public:
	Handler();
	~Handler();

	//behövs
	void createVertexBuffer(ID3D11Device* gDevice);
	void updateVertexBuffer(ID3D11DeviceContext* gDeviceContext);
	void CreateShaders(ID3D11Device* gDevice);
	void CreateTriangleData(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext,
		int collisionType, Player* player);
	//finns, men en del av det i denna måste läggas in
	void Render(ID3D11DeviceContext* gDeviceContext); 
	

	//finns nog redan i stora 
	D3D11_SUBRESOURCE_DATA initiateSubresources();
	void initiateMatrices();
	void updateMatrices(ID3D11DeviceContext* gDeviceContext, objMatrices* cameraMatrices);
	ID3D11Buffer* createConstantBuffer(UINT size, bool dynamic, bool CPUupdates,
		D3D11_SUBRESOURCE_DATA* pData, ID3D11Device* gDevice);
	void initiateThatThing(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext);
};

#endif // !
