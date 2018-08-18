#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace fb {
	class Camera {
	public:
		Camera(glm::vec3 pos, glm::vec3 lookAtPoint);
		Camera(glm::vec3 pos, glm::vec3 lookAtPoint, double fov, bool perspectiveProjection, int screenWidth, int screenHeight);
		void initializePerspective(double fov);
		void initializeOrthographic(double left, double right, double bottom, double top);

		glm::mat4 view;
		glm::mat4 proj;
	};

	
}
