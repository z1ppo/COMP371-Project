#include <vector>
#include <cstring>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "Renderer.h"
#include "TextureLoader.h"
#include "HealthBar.h"

unsigned int HealthTextureID;
unsigned int HealthVertexBufferID;
unsigned int HealthUVBufferID;
unsigned int HealthShaderID;
unsigned int HealthUniformID;
unsigned int oldShaderID;
unsigned int remainingHP;


void HealthBar::initializeHP(const char * texturePath)
{
	remainingHP = 3;
	// Initialize texture
	HealthTextureID = TextureLoader::LoadTexture(texturePath);

	// Initialize VBO
	glGenBuffers(1, &HealthVertexBufferID);
	glGenBuffers(1, &HealthUVBufferID);

	//Text2DShaderID = Renderer::LoadShaders("Text2DVertexShader.vertexshader", "Text2DVertexShader.fragmentshader");

	// Initialize shader
	setCurrentShader();

	// Initialize uniforms' IDs
	HealthUniformID = glGetUniformLocation(HealthShaderID, "myTextureSampler");

	// Restore original shader
	restoreOldShader();
}

void HealthBar::displayHP(int x, int y, int size){


	//return;
	// Initialize shader
	setCurrentShader();

	// Bind texture
	// Initialize uniforms' IDs
	HealthUniformID = glGetUniformLocation(HealthShaderID, "myTextureSampler");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, HealthTextureID);

	//unsigned int length = strlen(text);

	// Fill buffers
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
	for (unsigned int i = 0; i < remainingHP; i++)
	{
		glm::vec2 vertex_up_left = glm::vec2(x + i*size, y + size);
		glm::vec2 vertex_up_right = glm::vec2(x + i*size + size, y + size);
		glm::vec2 vertex_down_right = glm::vec2(x + i*size + size, y);
		glm::vec2 vertex_down_left = glm::vec2(x + i*size, y);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);
/*
		char character = text[i];
		float uv_x = (character % 16) / 16.0f;
		float uv_y = (character / 16) / 16.0f;

		glm::vec2 uv_up_left = glm::vec2(uv_x, 1.0f - uv_y);
		glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f / 16.0f, 1.0f - uv_y);
		glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / 16.0f, 1.0f - (uv_y + 1.0f / 16.0f));
		glm::vec2 uv_down_left = glm::vec2(uv_x, 1.0f - (uv_y + 1.0f / 16.0f));
*/

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
	}

	// Bind shader
	glUseProgram(HealthShaderID);


	//	glGenBuffers(1, &Text2DVertexBufferID);
	//	glGenBuffers(1, &Text2DUVBufferID);



	glBindBuffer(GL_ARRAY_BUFFER, HealthVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, HealthUVBufferID);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);


	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(HealthUniformID, 0);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, HealthVertexBufferID);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, HealthUVBufferID);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	//	glDisable(GL_BLEND);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	restoreOldShader();
}

void HealthBar::cleanUpHP(){

	// Delete buffers
	glDeleteBuffers(1, &HealthVertexBufferID);
	glDeleteBuffers(1, &HealthUVBufferID);

	// Delete texture
	glDeleteTextures(1, &HealthTextureID);

	// Delete shader
	glDeleteProgram(HealthShaderID);
}


void HealthBar::setCurrentShader()
{
	// Store old shader
	oldShaderID = Renderer::GetCurrentShader();

	// Initialize 2D Shader
	Renderer::SetShader(SHADER_2D);
	HealthShaderID = Renderer::GetShaderProgramID();
}

void HealthBar::restoreOldShader()
{
	// Restore old shader
	Renderer::SetShader((ShaderType)oldShaderID);
}

void HealthBar::removeHP(int amountToRemove)
{
	if (remainingHP > 0)
	{
		remainingHP = remainingHP - amountToRemove;
	}
	if (remainingHP == 0)
	{
		// player is dead, end game
	}
}

void HealthBar::addHP(int amountToAdd)
{
	if (remainingHP < 3)
	{
		remainingHP = remainingHP + amountToAdd;
	}
}

unsigned int HealthBar::GetRemainingHP(){
return remainingHP;
}