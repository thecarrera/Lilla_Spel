#ifndef PARTICLECLASS_H
#define PARTICLECLASS_H

#include "Includes.h"

//http://www.rastertek.com/dx11tut39.html //referens

class ParticleClass
{
private:
	//devices
	ID3D11Device* gDevice;
	ID3D11DeviceContext* gDeviceContext;

	//partikel randomizer grejer
	float mParticleDeviationX;
	float mParticleDeviationY;
	float mParticleDeviationZ;
	float mParticleVelocity;
	float mParticleVelocityVariation;
	float mParticleSize;
	float mParticlesPerSecond;
	int mMaxParticles;

	//counters
	int amountOfParticlers;
	float accumulatedTime;

	//texture
	ID3D11Resource* textureResoure;
	ID3D11ShaderResourceView* textureView;

	//array av partiklar
	ParticleType* particleArray;

	//info om verticerna
	int mVertexCount;
	int mIndexCount;
	vertexType* mVertices;

	//buffers
	ID3D11Buffer* mVertexBuffer;
	ID3D11Buffer* mIndexBuffer;


public:
	ParticleClass(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext);
	ParticleClass(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, const ParticleClass& particleClass);
	~ParticleClass();

	bool initialize(WCHAR* textureFileName);
	void kill();
	bool frameUpdate(float frameTime); // ska vi ha frame time?
	void render();
	//function för return texture
	void createTexture();




	int getmIndexCount();

};

#endif 
