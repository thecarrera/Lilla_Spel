#include "Particle.h"

Particle::Particle()
{
	this->particle.position = { 0, 0, 0 };
	this->particle.size = 0;
	this->directionVector = { 0, 0, 0 };
	this->velocityVector = { 0, 0, 0 };
	this->velocity = { 0, 0, 0 };
	srand(static_cast <unsigned> (time(0)));
	this->age = 0;
	this->gravity = 0;
	this->gravityIncrease = 0;
	this->lifetime = 0;
}

Particle::~Particle()
{

}

void Particle::createParticle(DirectX::XMFLOAT3 centrePoint, DirectX::XMFLOAT3 radiusRandomizer,
	DirectX::XMFLOAT3 directionVector_source, DirectX::XMFLOAT3 directionLowerRandomRange_source,
	DirectX::XMFLOAT3 directionUpperRandomRange_source, DirectX::XMFLOAT3 color, //COLOR MELLAN 0 - 1
	float lowerSizeRandomRange, float higherSizeRandomRange, float lifetime,
	DirectX::XMFLOAT3 velocity, float oppositeVelocityPercent, float gravityIncrease,
	int ID)
{
	std::mt19937 gen(rd());
	this->lifetime = lifetime;

	//Position
	std::uniform_real_distribution<> posXRand(-radiusRandomizer.x, radiusRandomizer.x);
	std::uniform_real_distribution<> posYRand(-radiusRandomizer.y, radiusRandomizer.y);
	std::uniform_real_distribution<> posZRand(-radiusRandomizer.z, radiusRandomizer.z);
	
	float posX = centrePoint.x + posXRand(gen);
	float posY = centrePoint.y + posYRand(gen);
	float posZ = centrePoint.z + posZRand(gen);

	particle.position = { posX, posY, posZ };

	//size
	std::uniform_real_distribution<> sizeRand(lowerSizeRandomRange, higherSizeRandomRange);
	this->particle.size = sizeRand(gen);
	this->size = this->particle.size;

	//color
	float finalcolorX = color.x / 256;
	float finalcolorY = color.y / 256;
	float finalcolorZ = color.z / 256;
	this->particle.color = { finalcolorX, finalcolorY, finalcolorZ, 1.0f };

	//vinkel	för att få en vinkel på 360 grader så ska x och y va samma random nummer åt både + och -
	DirectX::XMFLOAT3 directionVector = directionVector_source;

	DirectX::XMFLOAT3 directionLowerRandomRange = directionLowerRandomRange_source;

	DirectX::XMFLOAT3 directionUpperRandomRange = directionUpperRandomRange_source;

	std::uniform_real_distribution<> directionXRand(directionLowerRandomRange.x, directionUpperRandomRange.x);
	std::uniform_real_distribution<> directionYRand(directionLowerRandomRange.y, directionUpperRandomRange.y);
	std::uniform_real_distribution<> directionZRand(directionLowerRandomRange.z, directionUpperRandomRange.z);

	directionVector.x = directionVector.x + directionXRand(gen);
	directionVector.y = directionVector.y + directionYRand(gen);
	directionVector.z = directionVector.z + directionZRand(gen);

	this->directionVector = directionVector;

	//Calculate velocity
	this->velocityVector.x = this->directionVector.x * velocity.x;
	this->velocityVector.y = this->directionVector.y * velocity.y;
	this->velocityVector.z = this->directionVector.z * velocity.z;

	//Opposite velocity Vector
	this->oppositeVelocityVector.x = -this->velocityVector.x * oppositeVelocityPercent;
	this->oppositeVelocityVector.y = -this->velocityVector.y * oppositeVelocityPercent;
	this->oppositeVelocityVector.z = -this->velocityVector.z * oppositeVelocityPercent;

	//gravity
	this->gravityIncrease = gravityIncrease / 10000000000;

	//whatTriangle
	std::uniform_real_distribution<> switchTriangle(0, 5);
	int what = switchTriangle(gen);

	switch (what)
	{
	case 1:
		this->particle.whatTriangle = 0;
		break;
	case 2:
		this->particle.whatTriangle = 12;
		break;
	case 3:
		this->particle.whatTriangle = 24;
		break;
	case 4:
		this->particle.whatTriangle = 36;
		break;
	}

	//padding
	this->particle.padding = { 0, 0, 0 };

	//ID
	this->ID = ID;
	//ID = 1 -> triggerWaterfall
}

void Particle::updateParticle(float frameTime)
{
	this->age += frameTime;

	//gravity
	this->gravity -= gravityIncrease;
	this->velocityVector.y = this->velocityVector.y + this->gravity;

	//makes it smaller with time
	float newSize = (1 - (age / lifetime)) * this->size;
	particle.size = newSize;

	//opposite velocity
	this->oppositeVelocityVector.x = oppositeVelocityVector.x + (oppositeVelocityVector.x / 10000);
	this->oppositeVelocityVector.y = oppositeVelocityVector.y + (oppositeVelocityVector.y / 10000);
	this->oppositeVelocityVector.z = oppositeVelocityVector.z + (oppositeVelocityVector.z / 10000);

	//new Pos
	particle.position = { particle.position.x + this->velocityVector.x + this->oppositeVelocityVector.x,
						  particle.position.y + this->velocityVector.y + this->oppositeVelocityVector.y,
						  particle.position.z + this->velocityVector.z + this->oppositeVelocityVector.z };
}

hlslParticle Particle::getParticle()
{
	return this->particle;
}

float Particle::getAge()
{
	return this->age;
}

float Particle::getLifetime()
{
	return this->lifetime;
}

int Particle::getID()
{
	return this->ID;
}