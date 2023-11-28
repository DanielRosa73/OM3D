#include "SceneObject.h"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

namespace OM3D
{

    SceneObject::SceneObject(std::shared_ptr<StaticMesh> mesh, std::shared_ptr<Material> material)
        : _mesh(std::move(mesh))
        , _material(std::move(material))
    {
        canCull = _material->get_blend_mode() == BlendMode::Alpha;
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
        return glm::vec3(_transform[3][0], _transform[3][1], _transform[3][2]);
    }

} // namespace OM3D
