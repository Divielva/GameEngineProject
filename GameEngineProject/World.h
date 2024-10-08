#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "collections/QuadTree.h"

class SceneUpdatableObject;
class Arrow;

class World
{
    QuadTree<SceneUpdatableObject*> quad_tree;
    SceneUpdatableObject* Cubes[4];

public:
    World();
    ~World()
    {
        clear();
    }

    void insert(SceneUpdatableObject *object);

    void draw();

    void draw_debug(Line *line, Arrow *arrow);

    void set_bounds(const glm::vec3 &center, const glm::vec3 &extent);

    void update(float delta_time);

    void clear()
    {
		quad_tree.clear();
    }
};
