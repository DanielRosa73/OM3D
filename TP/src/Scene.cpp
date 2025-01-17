#include "Scene.h"

#include <TypedBuffer.h>

#include <shader_structs.h>

#include <iostream>

namespace OM3D
{

    Scene::Scene()
    {}

    void Scene::add_object(SceneObject obj)
    {
        _objects.emplace_back(std::move(obj));
    }

    void Scene::add_light(PointLight obj)
    {
        _point_lights.emplace_back(std::move(obj));
    }

    Span<const SceneObject> Scene::objects() const
    {
        return _objects;
    }

    Span<const PointLight> Scene::point_lights() const
    {
        return _point_lights;
    }

    Camera &Scene::camera()
    {
        return _camera;
    }

    const Camera &Scene::camera() const
    {
        return _camera;
    }

    void Scene::set_sun(glm::vec3 direction, glm::vec3 color)
    {
        _sun_direction = direction;
        _sun_color = color;
    }

    float pointPlaneDistance(glm::vec3 normal, glm::vec3 pos)
    {
        return glm::dot(normal, pos);
    }

    bool frustumCull(const SceneObject &obj, Frustum &frustum, const glm::mat4 &view, glm::vec3 cameraPos)
    {
        glm::vec3 pos = obj.getPosition() - cameraPos;
        float radius = obj.getCullRadius();

        // std::cout << " pos " << pos[0] << " " << pos[1] << " " << pos[2] << "\n";

        float tmp;
        tmp = pointPlaneDistance(frustum._near_normal, pos);
        // std::cout << " near " << tmp << "\n";
        if (tmp < -radius)
        {
            return 1;
        }
        tmp = pointPlaneDistance(frustum._top_normal, pos);
        // std::cout << " top " << tmp << "\n";
        if (tmp < -radius)
        {
            return 1;
        }
        tmp = pointPlaneDistance(frustum._bottom_normal, pos);
        // std::cout << " bot " << tmp << "\n";
        if (tmp < -radius)
        {
            return 1;
        }
        tmp = pointPlaneDistance(frustum._right_normal, pos);
        // std::cout << " right " << tmp << "\n";
        if (tmp < -radius)
        {
            return 1;
        }
        tmp = pointPlaneDistance(frustum._left_normal, pos);
        // std::cout << " left " << tmp << "\n";
        if (tmp < -radius)
        {
            return 1;
        }
        return 0;
    }

    void Scene::render(size_t &outNbCulled) const
    {
        // Fill and bind frame data buffer
        TypedBuffer<shader::FrameData> buffer(nullptr, 1);
        {
            auto mapping = buffer.map(AccessType::WriteOnly);
            mapping[0].camera.view_proj = _camera.view_proj_matrix();
            mapping[0].point_light_count = u32(_point_lights.size());
            mapping[0].sun_color = _sun_color;
            mapping[0].sun_dir = glm::normalize(_sun_direction);
        }
        buffer.bind(BufferUsage::Uniform, 0);

        // Fill and bind lights buffer
        TypedBuffer<shader::PointLight> light_buffer(nullptr, std::max(_point_lights.size(), size_t(1)));
        {
            auto mapping = light_buffer.map(AccessType::WriteOnly);
            for (size_t i = 0; i != _point_lights.size(); ++i)
            {
                const auto &light = _point_lights[i];
                mapping[i] = { light.position(), light.radius(), light.color(), 0.0f };
            }
        }
        light_buffer.bind(BufferUsage::Storage, 1);

        // Render every object
        outNbCulled = 0;
        Frustum f = _camera.build_frustum();
        for (const SceneObject &obj : _objects)
        {
            if (!frustumCull(obj, f, _camera.view_matrix(), _camera.position()))
            {
                obj.render();
            }
            else
            {
                outNbCulled += 1;
            }
        }
    }

} // namespace OM3D
