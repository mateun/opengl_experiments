#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace fb {
	class Camera {
	public:
		Camera(glm::vec3 p, glm::vec3 forward, glm::vec3 right, glm::vec3 up);
		Camera(glm::vec3 pos, glm::vec3 lookAtPoint);
		Camera(glm::vec3 pos, glm::vec3 lookAtPoint, double fov, bool perspectiveProjection, int screenWidth, int screenHeight);
		void initializePerspective(double fov);
		void initializeOrthographic(double left, double right, double bottom, double top);

		void updateCamera();

		void updateFromDirections();

		glm::vec3 pos;
		glm::mat4 view;
		glm::mat4 proj;

		glm::vec3 _forward;
		glm::vec3 _right;
		glm::vec3 _up;
		glm::vec3 _lookForward;
	};

	
}
