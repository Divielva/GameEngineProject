#include "AABB.h"
#include "../objects/debugTools/Line.h"
#include "../ShaderStore.h"
#include "../Material.h"
#include "../objects/base/GameObject.h"

AABB::AABB(glm::vec3 center, glm::vec3 extent) : center(center), extent(extent)
{
}
AABB::AABB() : center(glm::vec3(0.0f)), extent(glm::vec3(0.0f))
{
}
AABB::~AABB()
{
}

void AABB::draw_debug(Line* line)
{
    glm::vec3 min = center - extent;
    glm::vec3 max = center + extent;
    line->set_positions(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z));
    line->draw();
    line->set_positions(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, max.y, min.z));
    line->draw();
    line->set_positions(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, min.y, max.z));
    line->draw();
    line->set_positions(glm::vec3(max.x, max.y, max.z), glm::vec3(min.x, max.y, max.z));
    line->draw();
    line->set_positions(glm::vec3(max.x, max.y, max.z), glm::vec3(max.x, min.y, max.z));
    line->draw();
    line->set_positions(glm::vec3(max.x, max.y, max.z), glm::vec3(max.x, max.y, min.z));
    line->draw();
    line->set_positions(glm::vec3(min.x, max.y, min.z), glm::vec3(max.x, max.y, min.z));
    line->draw();
    line->set_positions(glm::vec3(min.x, max.y, min.z), glm::vec3(min.x, max.y, max.z));
    line->draw();
    line->set_positions(glm::vec3(min.x, min.y, max.z), glm::vec3(max.x, min.y, max.z));
    line->draw();
    line->set_positions(glm::vec3(min.x, min.y, max.z), glm::vec3(min.x, max.y, max.z));
    line->draw();
    line->set_positions(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, max.y, min.z));
    line->draw();
    line->set_positions(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, min.y, max.z));
    line->draw();
}

template <>
bool AABB::contains<GameObject*>(GameObject* const& point) const
{
    return contains(point->get_position());
}

void AABB::update(GameObject* object)
{
    center = object->get_position();
}

bool AABB::is_on_frustum(Frustum* frustum)
{
    return is_on_or_forward_plane(&frustum->left_face) &&
        is_on_or_forward_plane(&frustum->right_face) &&
        is_on_or_forward_plane(&frustum->top_face) &&
        is_on_or_forward_plane(&frustum->bottom_face) &&
        is_on_or_forward_plane(&frustum->near_face) &&
        is_on_or_forward_plane(&frustum->far_face);
}

bool AABB::is_on_or_forward_plane(Plane* plane)
{
    const float r = extent.x * abs(plane->normal.x) + extent.y * abs(plane->normal.y) + extent.z * abs(plane->normal.z);
    return -r <= plane->getSignedDistanceToPlane(center);
}