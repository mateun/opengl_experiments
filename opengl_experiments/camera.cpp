#include "stdafx.h"
#include "camera.h"


fb::Camera::Camera(glm::vec3 p, glm::vec3 forward, glm::vec3 right, glm::vec3 up)
{
	pos = p;
	_forward = forward;
	_right = right;
	_up = up;
	_lookForward = forward;
	updateFromDirections();
	_zFar = 450;
}

fb::Camera::Camera(glm::vec3 p, glm::vec3 lookAtPoint)
{
	pos = p;
	view = glm::lookAtRH(p, lookAtPoint, glm::vec3(0, 1, 0));
	_zFar = 450;
}

fb::Camera::Camera(glm::vec3 p, glm::vec3 lookAtPoint, double fov, bool perspectiveProjection, int screenWidth, int screenHeight)
{
	
	pos = p;
	view = glm::lookAtRH(p, lookAtPoint, glm::vec3(0, 1, 0));
	if (perspectiveProjection == true)
		proj = glm::perspectiveFovRH(fov, 2.0, 2.0, 0.1, _zFar);
	else
		proj = glm::orthoRH(0.0, (double)screenWidth, 0.0, (double)screenHeight, 0.1, _zFar);
	
	_zFar = 450;
}

void fb::Camera::initializePerspective(double fov)
{
	proj = glm::perspectiveFovRH(fov, 2.0, 2.0, 0.1, _zFar);
}

void fb::Camera::initializeOrthographic(double left, double right, double bottom, double top)
{
	proj = glm::orthoRH(left, right, bottom, top, 0.1, _zFar);
}

void fb::Camera::updateCamera()
{
	view = glm::lookAtRH(pos, glm::vec3(0,0,0), glm::vec3(0, 1, 0));
}

void fb::Camera::updateFromDirections()
{
	glm::vec3 target = pos + glm::normalize(_forward);
	view = glm::lookAtRH(pos, target, glm::normalize(_up));
}

