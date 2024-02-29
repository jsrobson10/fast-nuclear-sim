
#include "locations.hpp"
#include <glm/ext/matrix_transform.hpp>

using namespace Sim::Graphics;

const glm::mat4 Locations::monitors[7] = {
	(
		glm::translate(glm::mat4(1), glm::vec3(-2.9475, -1.7778 + 0.05, 3 - 0.05)) *
		glm::rotate(glm::mat4(1), glm::radians<float>(-90), glm::vec3(1, 0, 0)) *
		glm::rotate(glm::mat4(1), glm::radians<float>(-90), glm::vec3(0, 1, 0)) *
		glm::scale(glm::mat4(1), glm::vec3(1.9, 1.9, 1.9))
	),
	(
		glm::translate(glm::mat4(1), glm::vec3(-1.5 + 0.05, 3.9475, 3 - 0.05)) *
		glm::rotate(glm::mat4(1), glm::radians<float>(-90), glm::vec3(1, 0, 0)) *
		glm::scale(glm::mat4(1), glm::vec3(1.9, 1.9, 1.9))
	),
	(
		glm::translate(glm::mat4(1), glm::vec3(1 + 0.05, 3.9475, 3 - 0.05)) *
		glm::rotate(glm::mat4(1), glm::radians<float>(-90), glm::vec3(1, 0, 0)) *
		glm::scale(glm::mat4(1), glm::vec3(1.9, 1.9, 1.9))
	),
	(
		glm::translate(glm::mat4(1), glm::vec3(3.5 + 0.05, 3.9475, 3 - 0.05)) *
		glm::rotate(glm::mat4(1), glm::radians<float>(-90), glm::vec3(1, 0, 0)) *
		glm::scale(glm::mat4(1), glm::vec3(1.9, 1.9, 1.9))
	),
	(
		glm::translate(glm::mat4(1), glm::vec3(6 + 0.05, 3.9475, 3 - 0.05)) *
		glm::rotate(glm::mat4(1), glm::radians<float>(-90), glm::vec3(1, 0, 0)) *
		glm::scale(glm::mat4(1), glm::vec3(1.9, 1.9, 1.9))
	),
	(
		glm::translate(glm::mat4(1), glm::vec3(8.9475, 7.0/3.0 - 0.05, 3 - 0.05)) *
		glm::rotate(glm::mat4(1), glm::radians<float>(-90), glm::vec3(1, 0, 0)) *
		glm::rotate(glm::mat4(1), glm::radians<float>(90), glm::vec3(0, 1, 0)) *
		glm::scale(glm::mat4(1), glm::vec3(1.9, 1.9, 1.9))
	),
	(
		glm::translate(glm::mat4(1), glm::vec3(8.9475, -1.0/3.0 - 0.05, 3 - 0.05)) *
		glm::rotate(glm::mat4(1), glm::radians<float>(-90), glm::vec3(1, 0, 0)) *
		glm::rotate(glm::mat4(1), glm::radians<float>(90), glm::vec3(0, 1, 0)) *
		glm::scale(glm::mat4(1), glm::vec3(1.9, 1.9, 1.9))
	)
};

