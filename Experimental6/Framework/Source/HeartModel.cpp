

// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "HeartModel.h"
#include "ShipModel.h"
#include "objloader.hpp"
#include "Renderer.h"
//#include "texture.hpp"
//#include "SOIL.h"
#include "Projectile.h"
#include "Camera.h"
#include "World.h"
#include "Mesh.h"
#include "Model.h"
#include"sceneLoader.h"
#include"HealthBar.h"

// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>
using namespace glm;

HeartModel::HeartModel(int shipTextureID,glm::vec3 size) : Model()
{
	// Create Vertex Buffer for all the verices of the Cube
	//Vertex vertexBuffer[] = loadOBJ("../Assets/Model/Chair.obj" );
	// Create a vertex array
	mTextureID = shipTextureID;
	heartScene = World::GetInstance()->heartScene;


	
}

HeartModel::~HeartModel()
{
	// Free the GPU from the Vertex Buffer
	heartScene->~sceneLoader();
}

void HeartModel::Update(float dt)
{
	 mRotationAngleInDegrees += 90 * dt;
	//vec3 tempPosition = (glm::vec3(0,0,0.5));

	//mScaling = vec3(0.1, 0.1, 0.1*World::GetInstance()->stretchConstant);
	//mScaling += vec3(0, 0, 1)*World::GetInstance()->stretchConstant*dt;

	//mPosition.z -= tempPosition.z;

	//this->SetPosition(this->GetPosition() - (glm::vec3(0, 0, World::GetInstance()->selfRotationConstant/2)*dt));
	vec3 aim = World::GetInstance()->mModel[0]->GetPosition() - mPosition;
	aim = normalize(aim);
	this->SetPosition(this->GetPosition()+aim*dt*World::GetInstance()->selfRotationConstant * 0.5f);
	if(this->GetPosition().z < -2.0f){
		Reset();
	}
	else if (this->GetPosition().z < 2.0f)	{
		if (glm::distance(mPosition, World::GetInstance()->mModel[0]->GetPosition()) < 0.969){
			Collision();
		}
	}



	
	//	bullet.Update(dt);
	Model::Update(dt);
}
void HeartModel::Collision(){
		HealthBar::addHP(1);
		Reset();
}

void HeartModel::Spawn(glm::vec3 pos){
							
	
		this->SetPosition(pos);
}
void HeartModel::Reset(){
	this->SetPosition(vec3(5.0, 5.0, -4.0));
}

void HeartModel::Draw()
{
	// Draw the Vertex Buffer
	// Note this draws a unit Cube
	// The Model View Projection transforms are computed in the Vertex Shader
	ShaderType oldShader = (ShaderType)Renderer::GetCurrentShader();
	Renderer::SetShader(TEX_LIGHT2);
	Renderer::CheckForErrors();
    glUseProgram(Renderer::GetShaderProgramID());

    Renderer::CheckForErrors();
	//bullet.Draw();

	GLuint textureLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "mySamplerTexture");
	Renderer::CheckForErrors();
    glActiveTexture(GL_TEXTURE0);
	Renderer::CheckForErrors();
	unsigned int programID = Renderer::GetShaderProgramID();
	Renderer::CheckForErrors();
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	Renderer::CheckForErrors();
	glUniform1i(textureLocation, 0);	
	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");
	Renderer::CheckForErrors();
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);
	Renderer::CheckForErrors();
	GLuint VMatrixLocation = glGetUniformLocation(programID, "ViewTransform");
	Renderer::CheckForErrors();
	 const Camera* currentCamera = World::GetInstance()->GetCurrentCamera();
	Renderer::CheckForErrors();
	mat4 V = currentCamera->GetViewMatrix();
	Renderer::CheckForErrors();
    glUniformMatrix4fv(VMatrixLocation, 1, GL_FALSE, &V[0][0]);
	Renderer::CheckForErrors();
	GLuint PMatrixLocation = glGetUniformLocation(programID, "ProjectionTransform");
	Renderer::CheckForErrors();
	mat4 P = currentCamera->GetProjectionMatrix();
	Renderer::CheckForErrors();
	glUniformMatrix4fv(PMatrixLocation, 1, GL_FALSE, &P[0][0]);
	Renderer::CheckForErrors();
	

	
    // Send the view projection constants to the shader
   



		// Get a handle for Light Attributes uniform
	
	GLuint LightPositionID = glGetUniformLocation(programID, "WorldLightPosition");
	Renderer::CheckForErrors();
	GLuint LightColorID = glGetUniformLocation(programID, "lightColor");
	Renderer::CheckForErrors();
	GLuint LightAttenuationID = glGetUniformLocation(programID, "lightAttenuation");
	Renderer::CheckForErrors();

	// Get a handle for Material Attributes uniform
	GLuint MaterialAmbientID = glGetUniformLocation(programID, "materialAmbient");
	Renderer::CheckForErrors();
    GLuint MaterialDiffuseID = glGetUniformLocation(programID, "materialDiffuse");
	Renderer::CheckForErrors();
    GLuint MaterialSpecularID = glGetUniformLocation(programID, "materialSpecular");
	Renderer::CheckForErrors();
    GLuint MaterialExponentID = glGetUniformLocation(programID, "materialExponent");
	Renderer::CheckForErrors();

		// Set shader constants
	const float ka = 1.2f;
	Renderer::CheckForErrors();
	const float kd = 0.8f;
	Renderer::CheckForErrors();
	const float ks = 0.2f;
	Renderer::CheckForErrors();
	const float n = 90.0f;
	Renderer::CheckForErrors();
        glUniform1f(MaterialAmbientID, ka);
		Renderer::CheckForErrors();
        glUniform1f(MaterialDiffuseID, kd);
		Renderer::CheckForErrors();
        glUniform1f(MaterialSpecularID, ks);
		Renderer::CheckForErrors();
        glUniform1f(MaterialExponentID, n);
		Renderer::CheckForErrors();

		vec3 lightColor = World::GetInstance()->lightColor;
		Renderer::CheckForErrors();
	float lightKc = World::GetInstance()->lightKc;
	Renderer::CheckForErrors();
	float lightKl = World::GetInstance()->lightKl;
	Renderer::CheckForErrors();
	float lightKq = World::GetInstance()->lightKq;
	Renderer::CheckForErrors();
	vec4 lightPosition = World::GetInstance()->lightPosition;
	Renderer::CheckForErrors();
		glUniform4f(LightPositionID, lightPosition.x, lightPosition.y, lightPosition.z, lightPosition.w);
		Renderer::CheckForErrors();
		glUniform3f(LightColorID, lightColor.r, lightColor.g, lightColor.b);
		Renderer::CheckForErrors();
		glUniform3f(LightAttenuationID, lightKc, lightKl, lightKq);
		Renderer::CheckForErrors();

   			GLuint ExplosionCoefID = glGetUniformLocation(programID, "ExplosionCoef");
		glUniform1f(ExplosionCoefID, -1.0f);
	

	heartScene->draw(programID);

	
	

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


	
	Renderer::CheckForErrors();
    
    Renderer::SetShader(oldShader);
}

bool HeartModel::ParseLine(const std::vector<ci_string> &token)
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
