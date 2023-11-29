#ifndef STATICMESH_H
#define STATICMESH_H

#include <graphics.h>
#include <TypedBuffer.h>
#include <Vertex.h>

#include <vector>

namespace OM3D
{

    struct MeshData
    {
        std::vector<Vertex> vertices;
        std::vector<u32> indices;
    };

    class StaticMesh : NonCopyable
    {
    public:
        StaticMesh() = default;
        StaticMesh(StaticMesh &&) = default;
        StaticMesh &operator=(StaticMesh &&) = default;

        StaticMesh(const MeshData &data);
        const TypedBuffer<Vertex> &getVertices() const;
        const TypedBuffer<u32> &getIndices() const;

        void draw() const;

    private:
        TypedBuffer<Vertex> _vertex_buffer;
        TypedBuffer<u32> _index_buffer;
    };

} // namespace OM3D

#endif // STATICMESH_H
