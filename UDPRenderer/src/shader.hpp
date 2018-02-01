#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ee
{
    class Shader
    {
    public:
		Shader() :
			m_programID(0)
		{
		}

		~Shader()
		{
			glDeleteProgram(m_programID);
		}

        bool initialize(const std::string &vtxPath, const std::string &frgPath, const std::string &geomPath = "");

		Shader(Shader&& shader)
		{
			shader.m_programID = 0;
		}

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        void use() const
		{
			glUseProgram(m_programID);
		}

        void assignTexture(std::string name, int textureUnit)
		{
			int unit = textureUnit - GL_TEXTURE0;
			glUniform1i(glGetUniformLocation(m_programID, name.c_str()), unit);
		}

        void assignMat4(std::string name, const glm::mat4 &mat)
		{
			glm::mat4 mat4(mat);
			GLint loc = glGetUniformLocation(m_programID, name.c_str());
			glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat4));
		}

        void assignBool(std::string name, bool value)
		{
			glUniform1i(glGetUniformLocation(m_programID, name.c_str()), value);
		}

        void assignColor(std::string name, glm::vec3 color)
		{
			glUniform3f(glGetUniformLocation(m_programID, name.c_str()), color.x, color.y, color.z);
		}

        void assignColor(std::string name, glm::vec4 color)
		{
			GLint loc = glGetUniformLocation(m_programID, name.c_str());
			glUniform4f(loc, color.x, color.y, color.z, color.w);
		}

        void assignVec3(std::string name, glm::vec3 vec)
		{
			GLint loc = glGetUniformLocation(m_programID, name.c_str());
			glUniform3f(loc, vec.x, vec.y, vec.z);
		}

        void assignFloat(std::string name, float val)
		{
			GLint loc = glGetUniformLocation(m_programID, name.c_str());
			glUniform1f(loc, val);
		}

        void bindTexture(GLenum target, GLuint number, GLuint texture)
		{
			glActiveTexture(GL_TEXTURE0 + number);
			glBindTexture(target, texture);
		}

        void unbindTexture(GLenum target)
		{
			glBindTexture(target, 0);
		}

    private:
        GLuint m_programID;
    };
}