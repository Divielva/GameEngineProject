#pragma once

#include "Renderable.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../../colliders/Collider.h"

class SceneObject : public Renderable
{
private:
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
    std::vector<SceneObject *> children;
    ColliderBase *collider;

public:
    SceneObject(std::vector<Vertex> vertices, std::vector<unsigned> indices) : Renderable(vertices, indices), scale(1), rotation(glm::quat(1, 0, 0, 0)), position(0), collider(nullptr) {}
    ~SceneObject()
    {
        if (collider != nullptr)
            delete collider;
    }

    void set_position(glm::vec3 position)
    {
        this->position = position;
        if (collider != nullptr)
            collider->update(this);
    }
    /// @brief Sets the rotation of the object
    /// @param rotation The rotation of the object in quaternion
    void set_rotation(glm::quat rotation) { this->rotation = rotation; }
    /// @brief Sets the rotation of the object
    /// @param rotation The rotation of the object in euler angles
    void set_rotation(glm::vec3 rotation) { this->rotation = glm::quat(rotation); }
    /// @brief Sets the rotation of the object
    /// @param rotation The rotation of the object in normalized vector form
    void set_rotation_world_up(glm::vec3 rotation) { this->rotation = glm::quat(glm::normalize(rotation), glm::vec3(0, 1, 0)); }
    void set_scale(glm::vec3 scale)
    {
        this->scale = scale;
        if (collider != nullptr)
            collider->update(this);
    }
    glm::vec3 get_position() const { return position; }
    glm::quat get_rotation() const { return rotation; }
    glm::vec3 get_scale() const { return scale; }

    void translate(glm::vec3 translation)
    {
        position += translation;
        if (collider != nullptr)
            collider->update(this);
    }
    void rotate(glm::quat rotation) { this->rotation = rotation * this->rotation; }
    void scale_by(glm::vec3 scale)
    {
        this->scale *= scale;
        if (collider != nullptr)
            collider->update(this);
    }

    void add_child(SceneObject *child) { children.push_back(child); }
    void remove_child(SceneObject *child) { children.erase(std::remove(children.begin(), children.end(), child), children.end()); }
    std::vector<SceneObject *> get_children() const { return children; }

    template <typename T>
    void set_collider(Collider<T> *collider)
    {
        this->collider = collider;
        collider->set_parent(this);
        collider->update(this);
    }
    virtual ColliderBase *get_collider() { return collider; }

    void pre_render() const override
    {
        Renderable::pre_render();
        get_shader()->set_mat4("model", get_model_matrix());
    }

    void render() const override
    {
        Renderable::render();
        for (auto &child : children)
        {
            child->pre_render();
            child->render();
            child->post_render();
        }
    }

    glm::mat4x4 get_model_matrix() const
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = model * glm::mat4_cast(rotation);
        model = glm::scale(model, scale);
        return model;
    }

    glm::mat4x4 get_world_matrix(glm::mat4x4 parent) const
    {
        return parent * get_model_matrix();
    }
};