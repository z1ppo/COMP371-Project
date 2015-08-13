#include <vector>
#include <cstring>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "Renderer.h"
#include "TextureLoader.h"
#include "Icons2D.h"

#include <string.h>

struct Icon
{
	unsigned int textureID;
	unsigned int vertexBufferID;
	unsigned int UVBufferID;
	unsigned int shaderID;
	unsigned int uniformID;
	unsigned int origShaderID;
	unsigned int xPos;
	unsigned int yPos;
	unsigned int size;
};

std::vector<Icon> neutralIcons;

void Icons2D::initializeIcon(const char * texturePath1, const char * texturePath2, const char * texturePath3, const char * texturePath4)
{
	const char * tex[4] = { texturePath1, texturePath2, texturePath3, texturePath4 };

	for (int i = 0; i < 4; i++)
	{
		Icon iconIt;

		//Initialize texture id
		iconIt.textureID = TextureLoader::LoadTexture(tex[i]);

		// Initialize VBO
		glGenBuffers(1, &iconIt.vertexBufferID);
		glGenBuffers(1, &iconIt.UVBufferID);

		// Store old shader
		iconIt.origShaderID = Renderer::GetCurrentShader();

		// Initialize 2D Shader
		Renderer::SetShader(SHADER_2D);
		iconIt.shaderID = Renderer::GetShaderProgramID();

		// Initialize uniforms' IDs
		iconIt.uniformID = glGetUniformLocation(iconIt.shaderID, "myTextureSampler");

		// Restore original shader
		Renderer::SetShader((ShaderType)iconIt.origShaderID);

		neutralIcons.push_back(iconIt);
	}
}

void Icons2D::displayIcon(int element, int x, int y, int size){

	// Store old shader
	neutralIcons[element].origShaderID = Renderer::GetCurrentShader();

	// Initialize 2D Shader
	Renderer::SetShader(SHADER_2D);
	neutralIcons[element].shaderID= Renderer::GetShaderProgramID();

	// Bind texture
	// Initialize uniforms' IDs
	neutralIcons[element].uniformID = glGetUniformLocation(neutralIcons[element].shaderID, "myTextureSampler");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, neutralIcons[element].textureID);

	//unsigned int length = strlen(text);

	// Fill buffers
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;

	glm::vec2 vertex_up_left = glm::vec2(x, y + size);
	glm::vec2 vertex_up_right = glm::vec2(x + size, y + size);
	glm::vec2 vertex_down_right = glm::vec2(x + size, y);
	glm::vec2 vertex_down_left = glm::vec2(x , y);

	vertices.push_back(vertex_up_left);
	vertices.push_back(vertex_down_left);
	vertices.push_back(vertex_up_right);

	vertices.push_back(vertex_down_right);
	vertices.push_back(vertex_up_right);
	vertices.push_back(vertex_down_left);

	glm::vec2 uv_up_left = glm::vec2(1.0f, 0.0f);
	glm::vec2 uv_up_right = glm::vec2(1.0f, 1.0f);
	glm::vec2 uv_down_right = glm::vec2(0.0f, 1.0f);
	glm::vec2 uv_down_left = glm::vec2(0.0f, 0.0f);

	UVs.push_back(uv_up_left);
	UVs.push_back(uv_down_left);
	UVs.push_back(uv_up_right);

	UVs.push_back(uv_down_right);
	UVs.push_back(uv_up_right);
	UVs.push_back(uv_down_left);


	// Bind shader
	glUseProgram(neutralIcons[element].shaderID);


	//	glGenBuffers(1, &Text2DVertexBufferID);
	//	glGenBuffers(1, &Text2DUVBufferID);
	
	glBindBuffer(GL_ARRAY_BUFFER, neutralIcons[element].vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, neutralIcons[element].UVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);


	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(neutralIcons[element].uniformID, 0);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, neutralIcons[element].vertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, neutralIcons[element].UVBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	//	glDisable(GL_BLEND);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	// Restore old shader
	Renderer::SetShader((ShaderType)neutralIcons[element].origShaderID);
}