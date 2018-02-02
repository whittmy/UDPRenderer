#pragma once

#include <glm/gtc/type_ptr.hpp>
#include "../Drawable.hpp"
#include "../Textures/Texture.hpp"
#include "Mesh.hpp"

namespace ee
{
    class DrawableMesh : public Drawable
    {
    public:
		DrawableMesh(const float *data, int dataSize, const int *indices, int indicesSize, const float *position, const float *invPosition,
			const std::string &textPack, bool dynamic = false, int priority = 0);

        //DrawableMesh(const Mesh *mesh, const std::string &textPack, bool dynamic = false, int priority = 0);
		bool update(const float *data, int dataSize, const float *position, const float *invPosition)
		{
			if (m_dataSize != dataSize)
			{
				return false;
			}

			m_data = data;
			m_position = glm::make_mat4(position);
			m_invPosition = glm::make_mat4(invPosition);

			m_updated = true;

			return true;
		}

        ~DrawableMesh()
        {
            glDeleteVertexArrays(1, &m_VAO);
            glDeleteBuffers(1, &m_EBO);
            glDeleteBuffers(1, &m_VBO);
        }

		// For meshes that require textures (so I should probably do something about that...)
        void setTexture(std::vector<Texture> texture)
		{ 
			m_textures = texture; 
		}

        void draw() override;

    private:
        std::vector<Texture> m_textures;

		bool m_updated;

		glm::mat4 m_position;
		glm::mat4 m_invPosition;

		const float *m_data;
		const int m_dataSize;

		const int *m_indices;
		const int m_indicesSize;

        GLenum m_type;
        GLuint m_VAO;
        GLuint m_VBO;
        GLuint m_EBO;
    };
}