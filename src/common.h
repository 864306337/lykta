#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>

#define EPS 1e-4f
#define M_PI 3.14159265359
#define INV_PI 0.31830988618

namespace Lykta {

	struct Ray {
		glm::vec3 o;
		glm::vec3 d;
		glm::vec2 t;

		Ray() {};
		Ray(glm::vec3 orig, glm::vec3 dir, glm::vec2 tz) : o(orig), d(dir), t(tz) {}
	};

	struct Hit {
		glm::vec2 texcoord;
		glm::vec3 pos;
		glm::vec3 normal;
		unsigned material;
	};

	struct Triangle {
		unsigned x, y, z;

		Triangle() {};
		Triangle(unsigned x_, unsigned y_, unsigned z_) : x(x_), y(y_), z(z_) {}
	};


	struct Basis {
		glm::vec3 x, y, z;

		// http://jcgt.org/published/0006/01/01/
		Basis(const glm::vec3& n) {
			float sign = copysignf(1.0f, n.z);
			const float a = -1.0f / (sign + n.z);
			const float b = n.x * n.y * a;
			x = glm::vec3(1.0f + sign * n.x * n.x * a, sign * b, -sign * n.x);
			y = glm::vec3(b, sign + n.y * n.y * a, -n.y);
			z = n;
		}

		glm::vec3 toLocalSpace(const glm::vec3& v) {
			return glm::vec3(glm::dot(x, v), glm::dot(y, v), glm::dot(z, v));
		}

		glm::vec3 fromLocalSpace(const glm::vec3& v) {
			return v.x * x + v.y * y + v.z * z;
		}
	};

	inline glm::mat4 lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) {
		glm::vec3 z = glm::normalize(center - eye);
		glm::vec3 x = glm::normalize(glm::cross(z, up));
		glm::vec3 y = glm::normalize(glm::cross(x, z));

		glm::mat4 result = glm::mat4();
		result[0] = glm::vec4(x, 0);
		result[1] = glm::vec4(y, 0);
		result[2] = glm::vec4(z, 0);
		result[3] = glm::vec4(eye, 1);

		return result;
	}
}
