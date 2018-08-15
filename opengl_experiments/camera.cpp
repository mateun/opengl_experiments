#include "stdafx.h"
#include "camera.h"

fb::Camera::Camera(glm::vec3 pos, glm::vec3 lookAtPoint, double fov, bool perspectiveProjection, int screenWidth, int screenHeight)
{
	
	view = glm::lookAtRH(pos, lookAtPoint, glm::vec3(0, 1, 0));
	if (perspectiveProjection == true)
		proj = glm::perspectiveFovRH(fov, 2.0, 2.0, 0.1, 100.0);
	else
		proj = glm::orthoRH(0.0, (double)screenWidth, 0.0, (double)screenHeight, 0.1, 100.0);
	
}
