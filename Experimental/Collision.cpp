#include "Collision.h"
#include <glm/common.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;
bool SphereToSphere(glm::vec3 s1_Point, float s1_Radius, glm::vec3 s2_Point, float s2_Radius ){
	float diffrence = distance(s1_Point, s2_Point);
	float radiusSum  = s1_Radius + s2_Radius;
	return (diffrence < radiusSum);
};