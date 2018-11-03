#include "Integrator.hpp"

using namespace Lykta;

glm::vec3 BSDFIntegrator::evaluate(const Ray& ray, const std::shared_ptr<Scene> scene, RandomSampler* sampler) {
	glm::vec3 result = glm::vec3(0.f);
	glm::vec3 throughput = glm::vec3(1.f);
	Ray r = ray;

	while (true) {
		Lykta::Hit hit;
		if (!scene->intersect(r, hit)) {
			break;
		}

		const MaterialPtr material = scene->getMaterial(hit.geomID);

		if (maxComponent(material->getEmission()) > 0.f) {
			result += throughput * material->getEmission();
		}

		// RR
		float s = sampler->next();
		float success = fminf(0.75f, luminance(throughput));
		if (s < (1 - success)) break;
		throughput /= success;

		// Sample BSDF
		Basis basis = Basis(hit.normal);
		SurfaceInteraction si;
		si.uv = hit.texcoord;
		si.pos = hit.pos;
		si.wi = basis.toLocalSpace(-r.d);
		glm::vec3 color = material->sample(sampler->next2D(), si);
		glm::vec3 out = basis.fromLocalSpace(si.wo);

		throughput *= color;
		r = Ray(hit.pos, out);
	}

	return result;
}