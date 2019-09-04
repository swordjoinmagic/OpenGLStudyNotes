#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"

using namespace SJM;

glm::mat4x4 Camera::getViewMatrix() {
	return glm::lookAt(
		pos,pos+ front,up
	);
}

glm::mat4x4 Camera::getProjectionMatrix() {
	return glm::perspective(
		glm::radians(fovAngle),aspect,
		nearDistance,farDistance);
}