#ifndef PARTICLE_H
#define PARTICLE_H

#include "Includes.h"

#include <ctime>
#include <random>

struct hlslParticle
{
	DirectX::XMFLOAT3 position;
	float size;
	DirectX::XMFLOAT4 color;
	int whatTriangle;
	DirectX::XMFLOAT3 padding;
};

class Particle
{
private:
	hlslParticle particle;

	std::random_device rd;
	DirectX::XMFLOAT3 directionVector;
	DirectX::XMFLOAT3 velocityVector;
	DirectX::XMFLOAT3 oppositeVelocityVector;
	DirectX::XMFLOAT3 velocity;
	float age;
	float gravity;
	float lifetime;
	float size;
	float gravityIncrease; 
	int ID;

public:
	Particle();
	~Particle();
	void createParticle(DirectX::XMFLOAT3 centrePoint, DirectX::XMFLOAT3 radius,
		DirectX::XMFLOAT3 directionVector_source, DirectX::XMFLOAT3 directionLowerRandomRange_source,
		DirectX::XMFLOAT3 directionUpperRandomRange_source, DirectX::XMFLOAT3 color,
		float lowerSizeRandomRange, float higherRandomSizeRange, float lifetime,
		DirectX::XMFLOAT3 velocity, float oppositeVelocityPercent, float gravityIncrease,
		int ID);
	void updateParticle(float frameTime);

	hlslParticle getParticle();
	float getAge();
	float getLifetime();
	int getID();
};

#endif // !PARTICLE_H
