#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "collections/QuadTree.h"
#include "collections/OcTree.h"
#include "culling/Frustum.h"
#include "Light.h"
#include "ecs/ecs_map.h"

class GameObject;
class Arrow;

struct DrawCounts
{
    unsigned objects_culled;
    unsigned objects_filtered;
    unsigned objects_drawn;
};

class World
{
    OcTree<GameObject*> tree;
    std::vector<GameObject*> objects_non_colliders;
    PointLight* pointLights[4];
    DirectionalLight* directionalLight;
    SpotLight* spotLight;
    ECSGlobalMap ecs;

public:
    World() {}
    ~World()
    {
        clear();
    }

    void insert(GameObject* object);

    DrawCounts draw(Frustum* frustum);

    void draw_debug(Line* line, Arrow* arrow);

    void set_bounds(const glm::vec3& center, const glm::vec3& extent);

    void update(float delta_time);
    void set_point_light(PointLight* light, unsigned index);
    void set_directional_light(DirectionalLight* light);
    void set_spot_light(SpotLight* light);
    void update_point_light(unsigned index, std::function<void(PointLight*)> update);
    void update_directional_light(std::function<void(DirectionalLight*)> update);
    void update_spot_light(std::function<void(SpotLight*)> update);

    ECSGlobalMap* get_ecs() { return &ecs; }

    void clear()
    {
        tree.clear();
        for (auto object : objects_non_colliders)
        {
            delete object;
        }
        objects_non_colliders.clear();
        for (auto light : pointLights)
        {
            delete light;
        }
        delete directionalLight;
        delete spotLight;
    }
};
