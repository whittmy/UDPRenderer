#include "shader.hpp"

#include <sstream>
#include <fstream>
#include <iostream>
#include <cassert>

bool ee::Shader::initialize(const std::string &vtxPath, const std::string &frgPath, const std::string &geomPath)
{
    bool geomShader = !geomPath.empty();

	std::ifstream vtxStream, frgStream, geomStream;
	std::stringstream vtxCode, frgCode, geomCode;
	vtxStream.exceptions(std::ios_base::badbit | std::ios_base::failbit);
	frgStream.exceptions(std::ios_base::badbit | std::ios_base::failbit);

	// open and load code into stream
    try
    {
        vtxStream.open(vtxPath);
        vtxCode << vtxStream.rdbuf();
        vtxStream.close();
    }
    catch (std::ifstream::failure excp)
    {
        std::cout << "Failed to open vertex shader: " << vtxPath << std::endl;
        return false;
    }

	try
	{
		frgStream.open(frgPath);
		frgCode << frgStream.rdbuf();
		frgStream.close();
	}
	catch (std::ifstream::failure excp)
	{
        std::cout << "Failed to open fragment shader: " << frgPath << std::endl;
        return false;
	}

    if (geomShader)
    {
        try
        {
            geomStream.open(geomPath);
            geomCode << geomStream.rdbuf();
            geomStream.close();
        }
        catch (std::ifstream::failure excp)
        {
            std::cout << "Failed to open geometry shader: " << geomPath << std::endl;
            return false;
        }
    }

	GLuint vtxID, frgID, geomID;
	GLint resultChk;
	GLchar resultLog[512];

	vtxID = glCreateShader(GL_VERTEX_SHADER);
	frgID = glCreateShader(GL_FRAGMENT_SHADER);
    if (geomShader)
    {
        geomID = glCreateShader(GL_GEOMETRY_SHADER);
    }
	
	std::string strTmpCode = std::move(vtxCode.str());
	const GLchar *ptrCode = strTmpCode.c_str();
	glShaderSource(vtxID, 1, &ptrCode, nullptr);
	glCompileShader(vtxID);

	glGetShaderiv(vtxID, GL_COMPILE_STATUS, &resultChk);
	if (!resultChk)
	{
		glGetShaderInfoLog(vtxID, 512, nullptr, resultLog);
        std::cout << "Issue when compiling vertex shader: " << vtxPath << std::endl;
		std::cout << resultLog << std::endl;
        return false;
	}

	strTmpCode = std::move(frgCode.str());
	ptrCode = strTmpCode.c_str();
	glShaderSource(frgID, 1, &ptrCode, nullptr);
	glCompileShader(frgID);

	glGetShaderiv(frgID, GL_COMPILE_STATUS, &resultChk);
	if (!resultChk)
	{
		glGetShaderInfoLog(frgID, 512, nullptr, resultLog);
        std::cout << "Issue when compiling fragment shader: " << frgPath << std::endl;
		std::cout << resultLog << std::endl;
        return false;
	}

    if (geomShader)
    {
        strTmpCode = std::move(geomCode.str());
        ptrCode = strTmpCode.c_str();
        glShaderSource(geomID, 1, &ptrCode, nullptr);
        glCompileShader(geomID);

        glGetShaderiv(geomID, GL_COMPILE_STATUS, &resultChk);
        if (!resultChk)
        {
            glGetShaderInfoLog(geomID, 512, nullptr, resultLog);
            std::cout << "Issue when compiling geometry shader: " << geomPath << std::endl;
            std::cout << resultLog << std::endl;
            return false;
        }
    }

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vtxID);
	glAttachShader(m_programID, frgID);
    if (geomShader)
    {
        glAttachShader(m_programID, geomID);
    }

	glLinkProgram(m_programID);
	glGetProgramiv(m_programID, GL_LINK_STATUS, &resultChk);
	if (!resultChk)
	{
		glGetProgramInfoLog(m_programID, 512, nullptr, resultLog);
        std::cout << "Issue when linking shaders: " << "vert: " << vtxPath << ", frag: " << frgPath << ", geom: " << geomPath << std::endl;
        std::cout << resultLog << std::endl;
        return false;
	}

	glDeleteShader(vtxID);
	glDeleteShader(frgID);
    if (geomShader)
    {
        glDeleteShader(geomID);
    }

    return true;
}