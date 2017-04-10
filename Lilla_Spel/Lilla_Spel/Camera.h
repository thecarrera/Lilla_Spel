#pragma once

#include "Includes.h"

class Camera
{
private:
	objMatrices cameraMatrices;
public:
	Camera(objMatrices cameraMatrices);
	~Camera();

};