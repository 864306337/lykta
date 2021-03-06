#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "common.h"

namespace Lykta {

	class Sampling {
	public:
		static inline glm::vec2 uniformDisk(const glm::vec2& sample) {
			float r = sqrtf(sample.x);
			float theta = sample.y * M_PI * 2;
			return glm::vec2(r * cosf(theta), r * sinf(theta));
		}

		static inline glm::vec3 cosineHemisphere(const glm::vec2& sample) {
			glm::vec2 disk = uniformDisk(sample);
			return glm::vec3(disk.x, disk.y, sqrtf(1 - disk.x * disk.x - disk.y * disk.y));
		}

		static inline float cosineHemispherePdf(const glm::vec3& v) {
			if (v.z < 0) return 0;
			return v.z * INV_PI;
		}

		// Inverse sampling according to D
		static inline glm::vec3 GGX(const glm::vec2& sample, float alpha) {
			float a2 = alpha * alpha;
			float theta = acosf(sqrtf((1 - sample.x) / (sample.x * (a2 - 1) + 1)));
			float phi = 2 * M_PI * sample.y;
			return glm::vec3(sinf(theta) * cosf(phi), sinf(theta) * sinf(phi), cosf(theta));
		}

		static inline float GGXPdf(const glm::vec3& wh, const glm::vec3& wi, float alpha) {
			float nh = localCosTheta(wh);
			float a2 = alpha * alpha;
			float tmp = nh * nh * (a2 - 1) + 1;
			float D = a2 / (M_PI * tmp * tmp);
			return nh * D / (4 * glm::dot(wi, wh));
		}

		static inline glm::vec3 uniformTriangle(const glm::vec2& sample) {
			float su1 = sqrtf(sample.x);
			float u = 1.f - su1, v = sample.y * su1;
			return glm::vec3(u, v, 1.f - u - v);
		}

		static inline glm::vec3 uniformCylinder(const glm::vec2& sample) {
			float wz = 2 * sample.x - 1;
			float theta = 2 * M_PI * sample.y;
			return glm::vec3(std::cos(theta), std::sin(theta), wz);
		}

		static inline glm::vec3 uniformSphere(const glm::vec2& sample) {
			glm::vec3 cylinder = uniformCylinder(sample);
			float r = sqrtf(1 - cylinder.z * cylinder.z);
			return glm::vec3(r * cylinder.x, r * cylinder.y, cylinder.z);
		}

		static inline float uniformSpherePdf(const glm::vec3& dir) {
			return 1.f / (4 * M_PI);
		}
	};
}
