//#include "ParticleClass.h"
//
//ParticleClass::ParticleClass(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext)
//{
//	particles = new Particle[];
//
//	this->gDevice = gDevice;
//	this->gDeviceContext = gDeviceContext;
//}
//
//ParticleClass::ParticleClass(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, const ParticleClass& particleClass)
//{
//	this->gDevice = gDevice;
//	this->gDeviceContext = gDeviceContext;
//}
//
//ParticleClass::~ParticleClass()
//{
//
//}

//bool ParticleClass::initialize(WCHAR* textureFileName)
//{
//	bool result = true;
//
//	//partiklesystem initialize
//	result = initializeParticleSystem();
//	if (!result)
//	{
//		return false;
//		//partikelsystemet
//	}
//
//	//partikel buffer
//	result = initializeBuffers();
//	if (!result)
//	{
//		return false;
//	}
//
//	//lyckades med allt
//	return true;
//}
//
//void ParticleClass::kill()
//{
//	//bort med buffer, är tydligen en sak?
//	shutdownBuffers();
//
//	//bort med partikelsystemet
//	shutdownParticleSystem();
//
//	//bort med texturen
//	releaseTexture();
//}
//
//bool ParticleClass::frameUpdate(float frameTime)
//{
//	bool result = true;
//
//	//bort med gamla partiklar
//	killParticles();
//
//	//nya partiklar
//	emitParticles(frameTime);
//
//	//updatera positionen av partiklarna
//	updateParticles(frameTime);
//
//	//måste uppdatera buffern om uppdaterar positionen
//	result = updateBuffers(this->gDeviceContext);
//	if (!result)
//	{
//		return false;
//	}
//
//	return true;
//}
//
//void ParticleClass::render()
//{
////	//vertex & index buffern ska uppdateras, vi gör det härifrån
////	renderBuffers(this->gDeviceContext);
////}
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
//void ParticleClass::initializeBuffers()
//{
//	unsigned long* indices;
//	int i;
//	HRESULT result;
//
//	this->mVertexCount = this->m_maxParticles * 12; //För vi ska ha 4 trianglar
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
//	//DETTA SKALL ÄNDRAS SEN
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
//	}
//	int index = 0;
//	bool found = false;
//	while (!found)
//	{
//		if ((this->m_particleList[index].active == false) || (this->m_particleList[index].PositionZ < posZ))
//		{
//			found = true;
//		}
//		else
//		{
//			index++;
//		}
//	}
//
//	int i = 0; 
//	int j = 0;
//}
//
//
//
//
//
//
//int ParticleClass::getmIndexCount()
//{
//	return this->mIndexCount;
//}