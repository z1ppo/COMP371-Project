//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "ChairModel.h"
#include "objloader.hpp"
#include "Renderer.h"
//#include "texture.hpp"
//#include "SOIL.h"
#include "Projectile.h"
#include "Camera.h"
#include "World.h"


// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>

using namespace glm;

ChairModel::ChairModel(int shipTextureID,vec3 size) : Model()
{
	// Create Vertex Buffer for all the verices of the Cube
	//Vertex vertexBuffer[] = loadOBJ("../Assets/Model/Chair.obj" );
	// Create a vertex array
	mTextureID = shipTextureID;
	
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals; // Won't be used at the moment.
	bool res = loadOBJ("../Assets/Models/Ship1.obj", vertices, uvs, normals);
	glGenVertexArrays(1, &mVertexArrayID);

	// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
	glGenBuffers(1, &mVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	
}

ChairModel::~ChairModel()
{
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);
}

void ChairModel::Update(float dt)
{
	// If you are curious, un-comment this line to have spinning cubes!
	// That will only work if your world transform is correct...
	// mRotationAngleInDegrees += 90 * dt; // spins by 90 degrees per second
	this->SetPosition(this->GetPosition()+(vec3(0,0,0.5)*dt));
	bullet.Update(dt);
	Model::Update(dt);
}

void ChairModel::Draw()
{
	// Draw the Vertex Buffer
	// Note this draws a unit Cube
	// The Model View Projection transforms are computed in the Vertex Shader
	ShaderType oldShader = (ShaderType)Renderer::GetCurrentShader();
	Renderer::SetShader(TEX_LIGHT2);
    glUseProgram(Renderer::GetShaderProgramID());

    Renderer::CheckForErrors();
	//bullet.Draw();

	GLuint textureLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "mySamplerTexture");
    glActiveTexture(GL_TEXTURE0);
	unsigned int programID = Renderer::GetShaderProgramID();
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glUniform1i(textureLocation, 0);	
	glBindVertexArray(mVertexArrayID);
	glBindVertexArray(uvbuffer);
	glBindVertexArray(normalbuffer);

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform"); 
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);
	GLuint VMatrixLocation = glGetUniformLocation(programID, "ViewTransform");
    // Send the view projection constants to the shader
    const Camera* currentCamera = World::GetInstance()->GetCurrentCamera();
	mat4 V = currentCamera->GetViewMatrix();
    glUniformMatrix4fv(VMatrixLocation, 1, GL_FALSE, &V[0][0]);
	GLuint PMatrixLocation = glGetUniformLocation(programID, "ProjectionTransform");
	mat4 P = currentCamera->GetProjectionMatrix();
	glUniformMatrix4fv(PMatrixLocation, 1, GL_FALSE, &P[0][0]);



		// Get a handle for Light Attributes uniform
	
	GLuint LightPositionID = glGetUniformLocation(programID, "WorldLightPosition");
	GLuint LightColorID = glGetUniformLocation(programID, "lightColor");
	GLuint LightAttenuationID = glGetUniformLocation(programID, "lightAttenuation");

	// Get a handle for Material Attributes uniform
	GLuint MaterialAmbientID = glGetUniformLocation(programID, "materialAmbient");
    GLuint MaterialDiffuseID = glGetUniformLocation(programID, "materialDiffuse");
    GLuint MaterialSpecularID = glGetUniformLocation(programID, "materialSpecular");
    GLuint MaterialExponentID = glGetUniformLocation(programID, "materialExponent");

		// Set shader constants
	const float ka = 0.2f;
	const float kd = 0.8f;
	const float ks = 0.2f;
	const float n = 90.0f;
        glUniform1f(MaterialAmbientID, ka);
        glUniform1f(MaterialDiffuseID, kd);
        glUniform1f(MaterialSpecularID, ks);
        glUniform1f(MaterialExponentID, n);

		vec3 lightColor = World::GetInstance()->lightColor;
	float lightKc = World::GetInstance()->lightKc;
	float lightKl = World::GetInstance()->lightKl;
	float lightKq = World::GetInstance()->lightKq;
	vec4 lightPosition = World::GetInstance()->lightPosition;
		glUniform4f(LightPositionID, lightPosition.x, lightPosition.y, lightPosition.z, lightPosition.w);
		glUniform3f(LightColorID, lightColor.r, lightColor.g, lightColor.b);
		glUniform3f(LightAttenuationID, lightKc, lightKl, lightKq);

    
	
	// 1st attribute buffer : vertex Positions
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(	0,				// attribute. No particular reason for 0, but must match the layout in the shader.
							3,				// size
							GL_FLOAT,		// type
							GL_FALSE,		// normalized?
							0, // stride
							(void*)0        // array buffer offset
						);
	
	// 2nd attribute buffer : vertex normal
	//glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	//glVertexAttribPointer(	1,
	//						3,
	//						GL_FLOAT,
	//						GL_FALSE,
	//						sizeof(Vertex),
	//						(void*)sizeof(vec3)    // Normal is Offseted by vec3 (see class Vertex)
	//					);
	
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			1,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			2,                                // attribute
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

	// 3rd attribute buffer : vertex color
	//glEnableVertexAttribArray(2);
	//glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	//glVertexAttribPointer(	2,
	//						2,
	//						GL_FLOAT,
	//						GL_FALSE,
	//						sizeof(Vertex),
	//						(void*) (2* sizeof(vec3)) // Color is Offseted by 2 vec3 (see class Vertex)
	//					);


	// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, 7710); //856, 4398 36 vertices: 3 * 2 * 6 (3 per triangle, 2 triangles per face, 6 faces)
	//glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	Renderer::CheckForErrors();
    
    Renderer::SetShader(oldShader);
}

bool ChairModel::ParseLine(const std::vector<ci_string> &token)
{
	if (token.empty())
	{
		return true;
	}
	else
	{
		return Model::ParseLine(token);
	}
}
