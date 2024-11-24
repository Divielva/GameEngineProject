#include "World.h"
#include "objects/base/GameObject.h"
#include "objects/base/GameObject.h"
#include "collections/QuadTree.h"
#include "objects/debugTools/Line.h"
#include "objects/debugTools/Arrow.h"
#include "colliders/ColliderHandler.h"

glm::vec3 checkLoc = glm::vec3(0, 0, 0);

void World::insert(GameObject* object)
{
    object->attatch_to_world(this);
    object->register_ecs(&ecs);
    if (object->get_collider() == nullptr)
    {
        objects_non_colliders.push_back(object);
        return;
    }
    tree.insert(object);
}

DrawCounts World::draw(Frustum* frustum)
{
    std::vector<GameObject*> objects;
    DrawCounts counts = { 0, 0, 0 };
    auto tuple = tree.query_range(tree.get_bounds(), objects, frustum);
    counts.objects_culled = std::get<0>(tuple);
    counts.objects_filtered = std::get<1>(tuple);
    for (auto& object : objects)
    {
        if (object->get_collider()->is_on_frustum(frustum))
        {
            counts.objects_drawn++;
            object->draw();
        }
    }
    for (auto& object : objects_non_colliders)
    {
        counts.objects_drawn++;
        object->draw();
    }
    return counts;
}

void World::draw_debug(Line* line, Arrow* arrow)
{
    tree.draw_debug(line);
}

void World::set_bounds(const glm::vec3& center, const glm::vec3& extent)
{
    tree.set_bounds(center, extent);
}

void World::update(float delta_time)
{
    std::vector<GameObject*> objects;
    AABB bounds = tree.get_bounds();
    tree.query_range(bounds, objects);
    unsigned index = 0;
    for (auto& object : objects)
    {
        if (object != nullptr)
        {
            if (object->get_active())
            {
                auto model = object->get_model_matrix();
                auto minVertex = glm::vec3(model * glm::vec4(object->get_min_vertex().position, 1));
                auto maxVertex = glm::vec3(model * glm::vec4(object->get_max_vertex().position, 1));
                std::vector<GameObject*> in_range_objects;
                bounds.center = (minVertex + maxVertex) / 2.0f; // center of the object
                bounds.extent = (maxVertex - minVertex) * 2.0f; // double the size of the object
                tree.query_range(bounds, in_range_objects);
                bool collided = false;
            }
        }
        index++;
    }
    tree.recalculate();
}