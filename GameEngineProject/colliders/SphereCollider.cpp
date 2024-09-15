#include "SphereCollider.h"
#include "../objects/base/SceneObject.h"

void SphereCollider::update(SceneObject *object)
{
	auto scale = object->get_scale();
	auto max_scale = std::max(scale.x, std::max(scale.y, scale.z));
	radius = max_scale;
}

glm::vec3 SphereCollider::get_center()
{
	return get_parent()->get_position();
}