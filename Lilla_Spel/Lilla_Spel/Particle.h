#ifndef PARTICLE_H
#define PARTICLE_H
#include "Includes.h"

class Particle
{
private:
	float distance; //hastighet * livstid
	float lifestime;
	float speedReduction;
	float gravity;
	float size;
	float color;

	//form f�r hur triangeln ska g�ras
public:
	Particle();
	~Particle();

	void CreateTriangleData();
};


#endif // !PARTICLE_H
