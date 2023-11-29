#include "SceneObject.h"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include <limits>
#include <iostream>

namespace OM3D
{

    SceneObject::SceneObject(std::shared_ptr<StaticMesh> mesh, std::shared_ptr<Material> material)
        : _mesh(std::move(mesh))
        , _material(std::move(material))
    {
        canCull = _material->get_blend_mode() == BlendMode::Alpha;

        glm::vec3 min = glm::vec3(std::numeric_limits<float>::infinity());
        glm::vec3 max = glm::vec3(-1.0 * std::numeric_limits<float>::infinity());
        auto vertices = _mesh->getVertices().map_const();
        auto indices = _mesh->getIndices().map_const();
        for (auto i = 0; i < _mesh->getIndices().element_count(); i++)
        {
            auto v = vertices[indices[i]];
            if (v.position[0] < min[0])
            {
                min[0] = v.position[0];
            }
            if (v.position[1] < min[1])
            {
                min[1] = v.position[1];
            }
            if (v.position[2] < min[2])
            {
                min[2] = v.position[2];
            }

            if (v.position[0] > max[0])
            {
                max[0] = v.position[0];
            }
            if (v.position[1] > max[1])
            {
                max[1] = v.position[1];
            }
            if (v.position[2] > max[2])
            {
                max[2] = v.position[2];
            }
        }

        // glm::vec3 tmp = glm::vec3(max[0] - min[0], max[1] - min[1], max[2] - min[2]);
        _cullRadius = glm::length(max - min);
        _barycenter = 0.5f * (max - min);
        std::cout << _barycenter[0] << " " << _barycenter[1] << " " << _barycenter[2] << "\n";
        std::cout << " radius " << _cullRadius << "\n";
    }

    void SceneObject::render() const
    {
        if (!_material || !_mesh)
        {
            return;
        }

        _material->set_uniform(HASH("model"), transform());
        _material->bind();
        if (canCull)
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);
        }

        _mesh->draw();

        if (canCull)
        {
            glDisable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);
        }
    }

    void SceneObject::set_transform(const glm::mat4 &tr)
    {
        _transform = tr;
    }

    const glm::mat4 &SceneObject::transform() const
    {
        return _transform;
    }

    const glm::vec3 SceneObject::getPosition() const
    {
        // return glm::vec3(_transform[3][0], _transform[3][1], _transform[3][2]);
        return _barycenter + glm::vec3(_transform[3][0], _transform[3][1], _transform[3][2]);
        // return _barycenter;
    }

    float SceneObject::setCullRadius(float f)
    {
        _cullRadius = f;
        return _cullRadius;
    }

    float SceneObject::getCullRadius() const
    {
        return _cullRadius;
    }

} // namespace OM3D
