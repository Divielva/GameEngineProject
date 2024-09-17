#include "World.h"
#include "objects/base/SceneObject.h"
#include "objects/base/SceneUpdatableObject.h"
#include "collections/QuadTree.h"
#include "objects/primitives/Cube.h"
#include "objects/debugTools/Line.h"
#include "objects/debugTools/Arrow.h"
#include "colliders/ColliderHandler.h"
#include "ShaderStore.h"

World::World()
{
	Cubes[0] = new Cube();
	Cubes[1] = new Cube();
	Cubes[2] = new Cube();
	Cubes[3] = new Cube();
    Cubes[0]->set_shader(ShaderStore::get_shader("noLight"));
    Cubes[1]->set_shader(ShaderStore::get_shader("noLight"));
    Cubes[2]->set_shader(ShaderStore::get_shader("noLight"));
    Cubes[3]->set_shader(ShaderStore::get_shader("noLight"));
	auto mat = new ColorMaterial();
	mat->color = glm::vec4(.5, .5, .5, 1);
	Cubes[0]->set_material(mat);
	Cubes[1]->set_material(mat);
	Cubes[2]->set_material(mat);
	Cubes[3]->set_material(mat);
}

void World::insert(SceneUpdatableObject *object)
{
    quad_tree.insert(object);
}

void World::draw()
{
    std::vector<SceneUpdatableObject *> objects;
    quad_tree.query_range(quad_tree.get_bounds(), objects);
    for (auto &object : objects)
    {
        object->draw();
    }
    for (auto& cube : Cubes)
    {
        cube->draw();
    }
}

void World::draw_debug(Line *line, Arrow *arrow)
{
    quad_tree.draw_debug(line);
}

void World::set_bounds(const glm::vec3& center, const glm::vec3& extent)
{
    quad_tree.set_bounds(center, extent);
    auto pos = center;
    pos.x = extent.x + Cubes[0]->get_scale().x;
	Cubes[0]->set_position(pos);
	pos = center;
	pos.x = -extent.x - Cubes[1]->get_scale().x;
	Cubes[1]->set_position(pos);
	pos = center;
	pos.z = extent.z + Cubes[2]->get_scale().z;
	Cubes[2]->set_position(pos);
	pos = center;
	pos.z = -extent.z - Cubes[3]->get_scale().z;
	Cubes[3]->set_position(pos);

    auto scale = extent;
	scale.x = 1;
	scale.y = 1;
	Cubes[0]->set_scale(scale);
	Cubes[1]->set_scale(scale);

	scale = extent;
	scale.z = 1;
	scale.y = 1;
    scale.x = scale.x + 2;
	Cubes[2]->set_scale(scale);
	Cubes[3]->set_scale(scale);
}

void World::update(float delta_time)
{
    std::vector<SceneUpdatableObject*> objects;
    AABB bounds = quad_tree.get_bounds();
    quad_tree.query_range(bounds, objects);
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
                std::vector<SceneUpdatableObject*> in_range_objects;
                bounds.center = (minVertex + maxVertex) / 2.0f; // center of the object
                bounds.extent = (maxVertex - minVertex) * 2.0f; // double the size of the object
                quad_tree.query_range(bounds, in_range_objects);
                bool collided = false;
                for (auto& in_range_object : in_range_objects)
                {
                    if (in_range_object->get_active() && in_range_object != object)
                    {
                        // TODO: figure out why recalculating when balls overlap doesn't work properly (balls get stuck together for a few seconds before separating)
                        if (ColliderHandler::contains(in_range_object->get_collider(), object->get_collider()))
                            object->apply_collision(in_range_object);
                    }
                }
                if (!collided)
                {
                    object->pre_update(delta_time);
                    object->update(delta_time);
                }
                while (!quad_tree.get_bounds().contains(object->get_position()))
                {
                    // ball went out of bounds the last frame
                    auto pos = object->get_position();
                    bounds = quad_tree.get_bounds();
                    // find out how much the ball went out of bounds by in delta
                    auto col_delta = glm::clamp(pos, -bounds.extent, bounds.extent);
                    auto pos_delta = col_delta - pos;
                    // find which side the ball is closest to
                    auto normal = ColliderHandler::get_collision_normal(&bounds, object->get_collider());
                    // apply the collision
                    object->apply_collision(normal);
                    // move the ball back to the bounds based on delta
                    object->set_position(col_delta + pos_delta);
                }
            }
        }
        index++;
    }
    quad_tree.recalculate();
}