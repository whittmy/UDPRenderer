#include "DrawableMesh.hpp"

#include <iostream>

ee::DrawableMesh::DrawableMesh(const float *data, int dataSize, const int *indices, int indicesSize, const float *position, const float *invPosition, const std::string &textPack, bool dynamic, int priority) :
	Drawable(textPack, priority),
	m_data(data),
	m_dataSize(dataSize),
	m_indices(indices),
	m_indicesSize(indicesSize),
	m_position(glm::make_mat4(position)),
	m_invPosition(glm::make_mat4(invPosition)),
	m_updated(false),
	m_type(dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW)
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_dataSize, m_data, m_type);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicesSize, m_indices, m_type);

	// TODO: Add support with different arrays for the other data points
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//ee::DrawableMesh::DrawableMesh(const Mesh* const mesh, const std::string& textPack, const bool dynamic, const int priority) :
//    Drawable(textPack, priority), 
//    m_mesh(mesh),
//    m_updateCount(0),
//    m_type(dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW)
//{
//    const std::vector<Vertex>& updatedVertices = m_mesh->getVerticesData();
//    const std::vector<MeshFace>& updatedMeshFaces = m_mesh->getMeshFaceData();
//    castVertexBuffer(updatedVertices);
//    m_cachedMeshFaces.insert(m_cachedMeshFaces.end(), updatedMeshFaces.begin(), updatedMeshFaces.end());
//
//    glGenVertexArrays(1, &m_VAO);
//    glGenBuffers(1, &m_VBO);
//    glGenBuffers(1, &m_EBO);
//
//    glBindVertexArray(m_VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(FloatVertex) * m_cachedVertices.size(), m_cachedVertices.data(), m_type);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(MeshFace) * m_cachedMeshFaces.size(), m_cachedMeshFaces.data(), m_type);
//
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(FloatVertex), (void*)offsetof(FloatVertex, m_position));
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(FloatVertex), (void*)offsetof(FloatVertex, m_normal));
//    glEnableVertexAttribArray(2);
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(FloatVertex), (void*)offsetof(FloatVertex, m_textCoord));
//
//    glBindVertexArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//}

void ee::DrawableMesh::draw()
{
    // Only update this if the mesh wasn't update
    if (m_updated)
    {
		m_updated = false;

        // No point in updating this because it is likely not to be updated
        // m_cachedMeshFaces.insert(m_cachedMeshFaces.end(), updatedMeshFaces.begin(), updatedMeshFaces.end());

        // update the vertices
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_dataSize, m_data);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_cachedMeshFaces.size() * sizeof(MeshFace), m_cachedMeshFaces.data());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/
    }
    glBindVertexArray(m_VAO);

    const Camera* camera = Renderer::getCamera();

    Drawable::m_shader->use();

    m_texturePack->preDraw(Drawable::m_shader, &m_shaderMaterial, camera, &m_textures); // sets whatever values it may want to set

	glm::mat4 modelTrans = m_position;

    glm::mat4 lookAt = camera->viewMatrix();
    glm::mat4 perspective = Renderer::getPerspective();
    glm::mat4 trans = perspective * lookAt * modelTrans;

    // all shaders need this:
    Drawable::m_shader->assignMat4("u_posTrans", trans);
    Drawable::m_shader->assignMat4("u_model", modelTrans); // in case this is needed

    glDrawElements(GL_TRIANGLES, m_indicesSize, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    m_texturePack->postDraw();
}