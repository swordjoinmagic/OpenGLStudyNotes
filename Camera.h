#pragma once
#ifndef SJMCAMERA
#define SJMCAMERA

#include <glm/glm.hpp>

namespace SJM {
	class Camera {
	public:
		// 摄像机位置
		glm::vec3 pos;
		// 摄像机正面方向
		glm::vec3 front;
		// 摄像机指向上的位置
		glm::vec3 up;

		float fovAngle;
		float aspect;
		float nearDistance, farDistance;

		Camera(glm::vec3 pos,glm::vec3 front,glm::vec3 cameraUp,
			float fovAngle,float aspect,float nearDistance,float farDistance)
			:pos(pos),front(front),up(cameraUp),
			fovAngle(fovAngle),aspect(aspect),
			nearDistance(nearDistance),farDistance(farDistance){}
		glm::mat4x4 getViewMatrix();
		glm::mat4x4 getProjectionMatrix();
	};
}


#endif // !SJMCAMERA
