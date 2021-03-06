//#include "ParticleClass.h"
//
//ParticleClass::ParticleClass()
//{
//	particles = new Particle[5000];
//
//}
//
//ParticleClass::ParticleClass(const ParticleClass& particleClass)
//{
//
//}
//
//ParticleClass::~ParticleClass()
//{
//
//}
//
//void ParticleClass::initialize(ID3D11Device* gDevice)
//{
//	//partiklesystem initialize
//	initiateParticles();
//
//	//partikel buffer
//	initializeBuffers(gDevice);
//}
//
//void ParticleClass::kill()
//{
//	//bort med buffer, �r tydligen en sak?
//	shutdownBuffers();
//
//	//bort med partikelsystemet
//	shutdownParticleSystem();
//}
//
//void ParticleClass::frameUpdate(float frameTime, ID3D11DeviceContext* gDeviceContext)
//{
//	//bort med gamla partiklar
//	killParticles();
//
//	//nya partiklar
//	emitParticles(frameTime);
//
//	//updatera positionen av partiklarna
//	updateParticles(frameTime);
//
//	//m�ste uppdatera buffern om uppdaterar positionen
//	updateBuffers(gDeviceContext);
//}
//
//void ParticleClass::render(ID3D11DeviceContext* gDeviceContext) //gdeivvdces
//{
//	//vertex & index buffern ska uppdateras, vi g�r det h�rifr�n
//	renderBuffers(gDeviceContext);
//}
//
//void ParticleClass::initiateParticles()
//{
//	this->m_particleDeviationX = 0.5f;
//	this->m_particleDeviationY = 0.1f;
//	this->m_particleDeviationZ = 2.0f;
//
//	this->m_particleVelocity = 1.0f;
//	this->m_particleVelocityVariation = 2.0f;
//	
//	this->m_particleSize = 0.2f;
//	
//	this->m_particlesPerSecond = 250.0f;
//
//	this->m_maxParticles = 5000;
//
//	this->m_particleList = new ParticleType[m_maxParticles];
//
//	for (int i = 0; i < this->m_maxParticles; i++)
//	{
//		this->m_particleList[i].active = false;
//	}
//
//	this->amountOfParticlers = 0;
//	this->accumulatedTime = 0;
//}
//
//void ParticleClass::shutdownParticleSystem()
//{
//	if (this->m_particleList)
//	{
//		delete[] this->m_particleList;
//		this->m_particleList = 0;
//	}
//}
//
//void ParticleClass::initializeBuffers(ID3D11Device* gDevice)
//{
//	unsigned long* indices;
//	int i;
//	HRESULT result;
//
//	this->mVertexCount = this->m_maxParticles * 6; //F�r vi ska ha 4 trianglar s� 12
//
//	this->mVertices = new VertexType[this->mVertexCount];
//
//	//initiatar vertex arrayen till noll
//	memset(this->mVertices, 0, (sizeof(VertexType) * mVertexCount));
//
//
//	D3D11_BUFFER_DESC vertexBufferDesc;
//	memset(&vertexBufferDesc, 0, sizeof(vertexBufferDesc)); //zeroar memoriet
//	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	vertexBufferDesc.ByteWidth = sizeof(VertexType) * this->mVertexCount;
//
//	D3D11_SUBRESOURCE_DATA vertexData;
//	vertexData.pSysMem = this->mVertices;
//	gDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &this->mVertexBuffer);
//}
//
//void ParticleClass::shutdownBuffers()
//{
//	this->mVertexBuffer->Release();
//	this->mVertexBuffer = 0;
//}
//
//void ParticleClass::emitParticles(float frameTime)
//{
//	//partikel info
//	float posX;
//	float posY;
//	float posZ;
//	float speed;
//	float red;
//	float green;
//	float blue;
//
//	this->accumulatedTime += frameTime;
//
//	bool makeNewParticle = false;
//
//	if (this->accumulatedTime > (1000.0f / this->m_particlesPerSecond))
//	{
//		this->accumulatedTime = 0.0f;
//		makeNewParticle = true;
//	}
//
//	//DETTA SKALL �NDRAS SEN
//	if ((makeNewParticle == true) && (this->amountOfParticlers < (this->m_maxParticles - 1)))
//	{
//		this->amountOfParticlers++;
//
//		posX = (((float)rand() - (float)rand()) / RAND_MAX) * m_particleDeviationX;
//		posY = (((float)rand() - (float)rand()) / RAND_MAX) * m_particleDeviationY;
//		posZ = (((float)rand() - (float)rand()) / RAND_MAX) * m_particleDeviationZ;
//
//		speed = m_particleVelocity + (((float)rand() - (float)rand()) / RAND_MAX) * m_particleVelocityVariation;
//
//		red = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
//		green = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
//		blue = (((float)rand() - (float)rand()) / RAND_MAX) + 0.5f;
//
//		int index = 0;
//		bool found = false;
//		while (!found)
//		{
//			if ((this->m_particleList[index].active == false) || (this->m_particleList[index].PositionZ < posZ)) //kanske ta bort den andra
//			{
//				found = true;
//			}
//			else
//			{
//				index++;
//			}
//		}
//
//		int i = this->amountOfParticlers;
//		int j = i - 1;
//
//		while (i != index)
//		{
//			this->m_particleList[i].PositionX = this->m_particleList[j].PositionX;
//			this->m_particleList[i].PositionY = this->m_particleList[j].PositionY;
//			this->m_particleList[i].PositionZ = this->m_particleList[j].PositionZ;
//			this->m_particleList[i].red = this->m_particleList[j].red;
//			this->m_particleList[i].green = this->m_particleList[j].green;
//			this->m_particleList[i].blue = this->m_particleList[j].blue;
//			this->m_particleList[i].velocity = this->m_particleList[j].velocity;
//			this->m_particleList[i].active = this->m_particleList[j].active;
//			i--;
//			j--;
//		}
//		//particlen ska in p� r�tt plats
//		this->m_particleList[index].PositionX = posX;
//		this->m_particleList[index].PositionY = posY;
//		this->m_particleList[index].PositionZ = posZ;
//		this->m_particleList[index].red = red;
//		this->m_particleList[index].green = green;
//		this->m_particleList[index].blue = blue;
//		this->m_particleList[index].velocity = speed;
//		this->m_particleList[index].active = true;
//	}
//}
//
//void ParticleClass::updateParticles(float frameTime)
//{
//
//	//f�r att flytta dem, bara i Y led
//	for (int i = 0; i < this->amountOfParticlers; i++)
//	{
//		this->m_particleList[i].PositionY = this->m_particleList[i].PositionY - (this->m_particleList[i].velocity * frameTime * 0.001f);
//	}
//}
//
//void ParticleClass::killParticles()
//{
//	//d�da pga h�jd
//	for (int i = 0; i < this->m_maxParticles; i++)
//	{
//		if ((this->m_particleList[i].active == true) && (this->m_particleList[i].PositionY < -3.0f))
//		{
//			this->m_particleList[i].active = false;
//			this->amountOfParticlers--;
//
//			for (int j = i; j < this->m_maxParticles - 1; j++)
//			{
//				this->m_particleList[j].PositionX = this->m_particleList[j + 1].PositionX;
//				this->m_particleList[j].PositionY = this->m_particleList[j + 1].PositionY;
//				this->m_particleList[j].PositionZ = this->m_particleList[j + 1].PositionZ;
//				this->m_particleList[j].red = this->m_particleList[j + 1].red;
//				this->m_particleList[j].green = this->m_particleList[j + 1].green;
//				this->m_particleList[j].blue = this->m_particleList[j + 1].blue;
//				this->m_particleList[j].velocity = this->m_particleList[j + 1].velocity;
//				this->m_particleList[j].active = this->m_particleList[j + 1].active;
//			}
//		}
//	}
//}
//
//void ParticleClass::updateBuffers(ID3D11DeviceContext* gDeviceContext)
//{
//	int index = 0;
//
//	//zeroar?
//	memset(this->mVertices, 0, (sizeof(VertexType) * this->mVertexCount));
//
//	//DETTA SKA �NDRAS!!!!
//	for (int i = 0; i < this->amountOfParticlers; i++)
//	{
//		// Bottom left.
//		this->mVertices[index].position = DirectX::XMFLOAT3(this->m_particleList[i].PositionX - m_particleSize, this->m_particleList[i].PositionY - this->m_particleSize, this->m_particleList[i].PositionZ);
//		this->mVertices[index].color = DirectX::XMFLOAT4(this->m_particleList[i].red, this->m_particleList[i].green, this->m_particleList[i].blue, 1.0f);
//	//	this->mVertices[index].FILL = DirectX::XMFLOAT2(0, 0);
//		index++;
//
//		// Top left.
//		this->mVertices[index].position = DirectX::XMFLOAT3(this->m_particleList[i].PositionX - m_particleSize, this->m_particleList[i].PositionY + m_particleSize, this->m_particleList[i].PositionZ);
//		this->mVertices[index].color = DirectX::XMFLOAT4(this->m_particleList[i].red, this->m_particleList[i].green, this->m_particleList[i].blue, 1.0f);
//	//	this->mVertices[index].FILL = DirectX::XMFLOAT2(0, 0);
//
//		index++;
//
//		// Bottom right.
//		this->mVertices[index].position = DirectX::XMFLOAT3(this->m_particleList[i].PositionX + m_particleSize, this->m_particleList[i].PositionY - m_particleSize, this->m_particleList[i].PositionZ);
//		this->mVertices[index].color = DirectX::XMFLOAT4(this->m_particleList[i].red, this->m_particleList[i].green, this->m_particleList[i].blue, 1.0f);
//	//	this->mVertices[index].FILL = DirectX::XMFLOAT2(0, 0);
//
//		index++;
//
//		// Bottom right.
//		this->mVertices[index].position = DirectX::XMFLOAT3(this->m_particleList[i].PositionX + m_particleSize, this->m_particleList[i].PositionY - m_particleSize, this->m_particleList[i].PositionZ);
//		this->mVertices[index].color = DirectX::XMFLOAT4(this->m_particleList[i].red, this->m_particleList[i].green, this->m_particleList[i].blue, 1.0f);
//	//	this->mVertices[index].FILL = DirectX::XMFLOAT2(0, 0);
//
//		index++;
//
//		// Top left.
//		this->mVertices[index].position = DirectX::XMFLOAT3(this->m_particleList[i].PositionX - m_particleSize, this->m_particleList[i].PositionY + m_particleSize, this->m_particleList[i].PositionZ);
//		this->mVertices[index].color = DirectX::XMFLOAT4(this->m_particleList[i].red, this->m_particleList[i].green, this->m_particleList[i].blue, 1.0f);
//	//	this->mVertices[index].FILL = DirectX::XMFLOAT2(0, 0);
//
//		index++;
//
//		// Top right.
//		this->mVertices[index].position = DirectX::XMFLOAT3(this->m_particleList[i].PositionX + m_particleSize, this->m_particleList[i].PositionY + m_particleSize, this->m_particleList[i].PositionZ);
//		this->mVertices[index].color = DirectX::XMFLOAT4(this->m_particleList[i].red, this->m_particleList[i].green, this->m_particleList[i].blue, 1.0f);
//	//	this->mVertices[index].FILL = DirectX::XMFLOAT2(0, 0);
//
//		index++;
//	}
//
//	D3D11_MAPPED_SUBRESOURCE mappedResource;
//	VertexType* verticesPtr;
//
//	//locka vertexbuffern
//	HRESULT hr = gDeviceContext->Map(this->mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//	if (FAILED(hr))
//	{
//		//Fail
//	}
//	
//	//verticesPtr = (VertexType*)mappedResource.pData;
//
//	memcpy(&mappedResource.pData, &this->mVertices, sizeof(this->mVertices)); //Krashar inte nu, men �r det r�tt?, kan �ven v�lja &this->mVertices[0]
//
//	// verticesPtr, (void*)
//
//	gDeviceContext->Unmap(this->mVertexBuffer, 0);
//}
//
//void ParticleClass::renderBuffers(ID3D11DeviceContext* gDeviceContext)
//{
//	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //elr 4 //Redan satt?
//
//	unsigned int stride = sizeof(this->mVertices);
//
//	//this->gDeviceContext->VSSetConstantBuffers(0, 1, &this->mVertexBuffer); //fast ska anv�nda den h�r under egentligen
//
//	//gDeviceContext->IASetVertexBuffers(0, 1, &this->mVertexBuffer, &stride, 0);
//
////	fortfarande error h�r
//}
//
//
//
//
//int ParticleClass::getmIndexCount()
//{
//	return this->mIndexCount;
//}