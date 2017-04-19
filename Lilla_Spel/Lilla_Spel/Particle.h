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

	//form för hur triangeln ska göras
public:
	Particle();
	~Particle();

	void CreateTriangleData();
};


#endif // !PARTICLE_H
