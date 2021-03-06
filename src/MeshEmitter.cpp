#include "Emitter.hpp"

using namespace Lykta;

glm::vec3 MeshEmitter::eval(EmitterInteraction& ei) const {
	float areaPDF = mesh->pdf();
	float dist = glm::length(ei.position - ei.origin);
	ei.pdf = areaPDF * (dist * dist) / fabsf(glm::dot(ei.normal, -ei.direction));
	return mesh->material->getEmission();
}

glm::vec3 MeshEmitter::sample(const glm::vec3& s, EmitterInteraction& ei) const {
	MeshSample info;
	mesh->sample(s, info);
	float areaPDF = info.pdf;
	float dist = glm::length(info.pos - ei.origin);

	ei.position = info.pos;
	ei.direction = glm::normalize(info.pos - ei.origin);
	ei.normal = info.normal;
	ei.shadowRay = Ray(ei.origin, ei.direction, glm::vec2(EPS, dist - EPS));
	
	float abso = abs(glm::dot(ei.normal, ei.direction));
	float areaToSolidAngle = dist * dist / abso;
	ei.pdf = areaToSolidAngle * areaPDF;
	return mesh->material->getEmission() / ei.pdf;
}