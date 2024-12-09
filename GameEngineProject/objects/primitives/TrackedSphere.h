#pragma once

#include "IcoSphere.h"
#include "../curves/BSpline.h"
#include "../../ShaderStore.h"
#include "../../Material.h"

class TrackedSphere : public IcoSphere
{
private:
	BSpline<glm::vec3>* track;
	float time = 0;
	float trackTime = 0.1f;

public:
	TrackedSphere() : IcoSphere()
	{
		track = new BSpline<glm::vec3>({}, 2, 10.0f);
		track->set_shader(ShaderStore::get_shader("no_light"));
		track->set_material(new ColorMaterial(glm::vec4(1, 0, 0, 1)));
		create(3);
	}

	virtual void update(float delta_time) override
	{
		if (!track->has_points())
			track->add_point(get_component<TransformComponent>()->position);
		time += delta_time;
		if (time > trackTime)
		{
			time = 0;
			track->add_point(get_component<TransformComponent>()->position);
		}
	}

	GameObject* get_track()
	{
		return track;
	}

};