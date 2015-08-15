//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "World.h"
#include "Renderer.h"
#include "ParsingHelper.h"

#include "StaticCamera.h"
#include "FirstPersonCamera.h"

#include "CubeModel.h"
#include "SphereModel.h"
#include "Animation.h"
#include "Billboard.h"
#include <GLFW/glfw3.h>
#include "EventManager.h"
#include "TextureLoader.h"

#include "RailCamera.h"
#include "ChairModel.h"
#include "ShipModel.h"
#include "ShipEnnemyModel.h"//TINO
#include "MeteorModel.h"
#include "Projectile.h"
#include "PlanetModel.h"
//#include "EarthModel.h"
//#include "MoonModel.h"
//#include "BackgroundSphere.h"
//#include "SunModel.h"
//#include "MarsModel.h"
#include "HeartModel.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>



#include "ParticleDescriptor.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"

#include "Text2D.h"
#include "HealthBar.h"
#include "Icons2D.h"


using namespace std;
using namespace glm;

World* World::instance;

	// 2D Text Alex
char timerText[256];
char scoreText[256];
char healthText[256];
char title[256]; // ALEX title menu ***********************
char lvlClearText[256]; // ALEX level clear***********************
char gameOverText[256]; // ALEX game over***********************

//float playerScore = 0;
// 2D Text Alex

static const GLfloat g_quad_vertex_buffer_data[] = { 
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
	};
World::World(int level)
{
	gameInProgress = false;
	playerDead = false;

	playerScore = 0;
	lvlDone = false;

	currentLevel = level;
	// 2DText Alex
	
	// Initialize 2d text class with appropriate texture
	Text2D::init2DText("../Assets/Fonts/Digital-7.tga");

	// Initialize health bar class with appropriate texture
	HealthBar::initializeHP("../Assets/Textures/Box_Green.png");
	//HealthBar::initializeHP("../Assets/Textures/screen.png");

	//Icons2D::initializeIcon("../Assets/Textures/screen.png", "../Assets/Textures/hp.png", "../Assets/Textures/time.png", "../Assets/Textures/score.png");
	
	// ALEX added menu texture******************************************
	Icons2D::initializeIcon("../Assets/Textures/menu.png", "../Assets/Textures/heart.png", "../Assets/Textures/clock.png", "../Assets/Textures/bullseye.png");
	
	// Initialize constant strings
	sprintf(lvlClearText, "LEVEL CLEARED!");
	sprintf(title, "INTERGALACTIC DEFENDERS");
	sprintf(gameOverText, "GAME OVER");
	// ALEX ******************************************



	// 2DText Alex

    instance = this;

//MAKS FIXED ROTATION AXIS
	if (level == 0){

		rotationAxis = vec3(0, 1, 0);
	}
	else if (level == 1){
		// ALEX MENU ******************************************
		gameInProgress = true;
		// ALEX MENU ******************************************
		rotationAxis = vec3(0, 1, 0);
	}else if (level == 2){
		// ALEX MENU ******************************************
		gameInProgress = true;
		// ALEX MENU ******************************************
		rotationAxis = vec3(-1, 0, 0);
	}
	


	orbitRotationConstantNumerator = 1;
	orbitRotationConstantDenominator = 18;

	stretchConstant = 1;

	selfRotationConstant = 10;

	// Setup Camera
	mCamera.push_back(new FirstPersonCamera(vec3(3.0f, 1.0f, 5.0f)));
	mCamera.push_back(new StaticCamera(vec3(3.0f, 30.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)));
//	mCamera.push_back(new StaticCamera(vec3(0.5f,  0.5f, 5.0f), vec3(0.0f, 0.5f, 0.0f), vec3(0.0f, 1.0f, 0.0f)));
	mCamera.push_back(new RailCamera(vec3(3.0f,  3.0f, 3.0f)));

	mCurrentCamera = 2;
	// Light Coefficients
	
	lightColor =  glm::vec3(1.0f, 1.0f, 1.0f);
	lightPosition = glm::vec4(2.0f, 0.0f, -2.0f, 0.0f);
	lightKc = 0.0f;
	lightKl = 0.0f;
	lightKq = 1.0f;
	// Material Coefficients



    
    // TODO: You can play with different textures by changing the billboardTest.bmp to another texture
#if defined(PLATFORM_OSX)
//    int billboardTextureID = TextureLoader::LoadTexture("Textures/BillboardTest.bmp");
    int billboardTextureID = TextureLoader::LoadTexture("Textures/Particle.png");
#else
//    int billboardTextureID = TextureLoader::LoadTexture("../Assets/Textures/BillboardTest.bmp");
	int billboardTextureID = TextureLoader::LoadTexture("../Assets/Textures/Particle.png");
	
#endif
    assert(billboardTextureID != 0);

    mpBillboardList = new BillboardList(2048, billboardTextureID);


		ParticleDescriptor* fountainDescriptor = new ParticleDescriptor();//Tino
     fountainDescriptor->SetFireDescriptorDamage();//Tino
	 emitter = new ParticleEmitter(vec3(0.0f, 0.0f, -100.0f));//Tino2
	ParticleSystem* ps = new ParticleSystem(emitter, fountainDescriptor);//Tino
	AddParticleSystem(ps);//Tino

	ParticleDescriptor* player_damage_2 = new ParticleDescriptor();//Tino2
     player_damage_2->SetFireDescriptorDamage2();//Tino2
	 emitter_damage_2 = new ParticleEmitter(vec3(0.0f, 0.0f, -100.0f));//Tino2
	 ParticleSystem* ps_damage_2 = new ParticleSystem(emitter_damage_2, player_damage_2);//Tino2
	 AddParticleSystem(ps_damage_2);//Tino2

     ParticleDescriptor* player_damage_3 = new ParticleDescriptor();//Tino2
     player_damage_3->SetFireDescriptorDamage3();//Tino2
	 emitter_damage_3 = new ParticleEmitter(vec3(0.0f, 0.0f, -100.0f));//Tino2
	 ParticleSystem* ps_damage_3 = new ParticleSystem(emitter_damage_3, player_damage_3);//Tino2
	 AddParticleSystem(ps_damage_3);//Tino2

     ParticleDescriptor* explosionDescriptor = new ParticleDescriptor();//Tino
     explosionDescriptor->SetRingExplosion();//Tino
	 emitter_ennemy = new ParticleEmitter(vec3(5.0f, 0.0f, -100.0f));//Tino
	 ParticleSystem* ps2 = new ParticleSystem(emitter_ennemy, explosionDescriptor);//Tino
	 AddParticleSystem(ps2);//Tino

	nextProjectile = 0;
	nextHeart = 0;
	spawntime = 0.0f;

	//Loads the textures
	shipTextureID = TextureLoader::LoadTexture("../Assets/Textures/ship3.jpg");
	droidTextureID = TextureLoader::LoadTexture("../Assets/Textures/droid2.tga");
	MeteorTextureID = TextureLoader::LoadTexture("../Assets/Textures/meteor.jpg");
	projTextureID = TextureLoader::LoadTexture("../Assets/Textures/projectile.jpg");
	moonTextureID = TextureLoader::LoadTexture("../Assets/Textures/moonHD.jpg");
//MAKS changed, different lvl gives different texture
	if(level == 0){
		spaceTextureID = TextureLoader::LoadTexture("../Assets/Textures/spaceTesto2.jpg");
		earthTextureID = TextureLoader::LoadTexture("../Assets/Textures/earthHD.jpg");
	} else if (level == 1){
		spaceTextureID = TextureLoader::LoadTexture("../Assets/Textures/spaceTesto2.jpg");
		earthTextureID = TextureLoader::LoadTexture("../Assets/Textures/earthHD.jpg");
	} else if (level == 2) {
		spaceTextureID = TextureLoader::LoadTexture("../Assets/Textures/spaceTesto2.jpg");
	} else if (level == 3) {
		spaceTextureID = TextureLoader::LoadTexture("../Assets/Textures/spaceTesto2.jpg");
	}
	sunTextureID = TextureLoader::LoadTexture("../Assets/Textures/sun.jpg");
	marsTextureID = TextureLoader::LoadTexture("../Assets/Textures/mars.jpg");
	heartTextureID = TextureLoader::LoadTexture("../Assets/Textures/heart.tga");


	 
	droidScene = new sceneLoader("../Assets/Models/droid.obj");
	meteorScene = new sceneLoader("../Assets/Models/meteor.obj");
	projScene = new sceneLoader("../Assets/Models/projectile.obj");
	planetScene = new sceneLoader("../Assets/Models/earthHD.obj");
	heartScene = new sceneLoader("../Assets/Models/heart.obj");




	FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, 1024, 768, 0,GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("Framebuffer  not complete");
	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
	ShaderType oldShader = (ShaderType)Renderer::GetCurrentShader();
	Renderer::SetShader(BLOOM);
	quad_programID = Renderer::GetShaderProgramID();
	Renderer::SetShader(oldShader);
	texID = glGetUniformLocation(quad_programID, "renderedTexture");
	shockTime = -0.7f;
}

World::~World()
{
	// Models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		delete *it;
	}

	mModel.clear();


	for (vector<Projectile*>::iterator it = mProjectile.begin(); it < mProjectile.end(); ++it)
	{
		delete *it;
	}

	mModel.clear();

	//Tino2 start
	for (vector<ParticleEmitter*>::iterator it = projEmmitter.begin(); it < projEmmitter.end(); ++it)
	{
		delete *it;
	}

	mModel.clear();

	for (vector<ShipEnnemyModel*>::iterator it = mShipEnnemyModel.begin(); it < mShipEnnemyModel.end(); ++it)
	{
		delete *it;
	}

	mModel.clear();

	for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
	{
		delete *it;
	}

	mAnimation.clear();

	for (vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
	{
		delete *it;
	}

	mAnimationKey.clear();

	// Camera
	for (vector<Camera*>::iterator it = mCamera.begin(); it < mCamera.end(); ++it)
	{
		delete *it;
	}
	mCamera.clear();

	delete mpBillboardList;
}

World* World::GetInstance()
{
    return instance;
}

void World::Update(float dt)
{

	//Dynamic light test

	quat rotQuatA = glm::angleAxis(0.0f, rotationAxis);
	quat rotQuatB = angleAxis(180.0f, rotationAxis);
	quat slerpedRotation = slerp(rotQuatA, rotQuatB, orbitRotationConstantNumerator * - dt / orbitRotationConstantDenominator);
	mat4 rotation = mat4_cast(slerpedRotation);
	lightPosition = rotation*lightPosition;

	// User Inputs
	spawntime+=dt;

	//Warp speed Activation

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W) == GLFW_PRESS && selfRotationConstant <= 50)
	{
		orbitRotationConstantNumerator += 0.1;
		selfRotationConstant += 1;
		stretchConstant += 0.1;
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S) == GLFW_PRESS && selfRotationConstant > 10)
	{
		orbitRotationConstantNumerator -= 0.1;
		selfRotationConstant -= 1;
		stretchConstant -= 0.1;
	}
	else {
		//stretchConstant = 0;
	}

	// 0 1 2 to change the Camera
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_1 ) == GLFW_PRESS)
	{
		mCurrentCamera = 0;
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_2 ) == GLFW_PRESS)
	{
		if (mCamera.size() > 1)
		{
			mCurrentCamera = 1;
		}
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_3 ) == GLFW_PRESS)
	{
		if (mCamera.size() > 2)
		{
			mCurrentCamera = 2;
		}
	}



	// Spacebar to change the shader
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_0 ) == GLFW_PRESS)
	{
		Renderer::SetShader(SHADER_SOLID_COLOR);
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_9 ) == GLFW_PRESS)
	{
		Renderer::SetShader(SHADER_BLUE);
	}

    // Update animation and keys
    for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
    {
        (*it)->Update(dt);
    }
    
    for (vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
    {
        (*it)->Update(dt);
    }


	// Update current Camera
	mCamera[mCurrentCamera]->Update(dt);

	// Update models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		(*it)->Update(dt);
	}
    for (vector<Projectile*>::iterator it = mProjectile.begin(); it < mProjectile.end(); ++it)
	{
		(*it)->Update(dt);
	}

	int i = 0;
	  for (vector<PlayerProjectile*>::iterator it = mPlayerProjectile.begin(); it < mPlayerProjectile.end(); ++it)
	{
		(*it)->Update(dt);
		projEmmitter[i]->setPosition((*it)->GetPosition());//Tino2
		i++;//Tino2
	}

	 for (vector<ShipEnnemyModel*>::iterator it = mShipEnnemyModel.begin(); it < mShipEnnemyModel.end(); ++it)
	{
		(*it)->Update(dt);
	}
    // Update billboards
    
    for (vector<ParticleSystem*>::iterator it = mParticleSystemList.begin(); it != mParticleSystemList.end(); ++it)
    {
        (*it)->Update(dt);
    }

		for (vector<HeartModel*>::iterator it = mHeartModel.begin(); it < mHeartModel.end(); ++it)
	{
		 (*it)->Update(dt);
	}
    
    mpBillboardList->Update(dt);

		// Update 2D Text

	//2D text Alex

	// For speed computation
	double lastTime = glfwGetTime();
	int nbFrames = 0;

	// Measure speed
	double currentTime = glfwGetTime();
	nbFrames++;
	if (currentTime - lastTime >= 1.0){ // If last printf() was more than 1sec ago
		// printf and reset
		printf("%f ms/frame\n", 1000.0 / double(nbFrames));
		nbFrames = 0;
		lastTime += 1.0;
	}

	sprintf(timerText, "time:%.f", glfwGetTime());

	// test HP addition and subtraction
	//
	//if (glfwGetTime() > 5.0f && glfwGetTime() < 5.5f)
	//{
	//	HealthBar::removeHP(1);
	//}
	//else if (glfwGetTime() > 10.0f && glfwGetTime() < 10.5f)
	//{
	//	HealthBar::addHP(1);
	//}

	// 2D Text Alex

	if (mShipModel->ExplosionCoef > 0 && shockTime < 3.0f){
		
			//shockTime = dt;
			shockTime += dt;
			if(shockTime > 2.0f ){
			shockTime = dt;
			}

		
	}


}

void World::Draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glViewport(0,0,1024,768); // Render on the whole framebuffer, complete from the lower left corner to the upper right

	Renderer::BeginFrame();
	

	


	// Set shader to use
	// Set shader to use
	glUseProgram(Renderer::GetShaderProgramID());

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");

	// Send the view projection constants to the shader
	mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	// Draw models

	//glFrustum(1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f, 1000.0f);
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		(*it)->Draw();
	}
	mShipModel->Draw();
	for (vector<ShipEnnemyModel*>::iterator it = mShipEnnemyModel.begin(); it < mShipEnnemyModel.end(); ++it)
	{
		(*it)->Draw();
	}
	for (vector<Projectile*>::iterator it = mProjectile.begin(); it < mProjectile.end(); ++it)
	{
		(*it)->Draw();
	}

	for (vector<PlayerProjectile*>::iterator it = mPlayerProjectile.begin(); it < mPlayerProjectile.end(); ++it)
	{
		(*it)->Draw();
	}
	for (vector<HeartModel*>::iterator it = mHeartModel.begin(); it < mHeartModel.end(); ++it)
	{
		(*it)->Draw();
	}
	//// Draw Path Lines
	//
	//// Set Shader for path lines
	//unsigned int prevShader = Renderer::GetCurrentShader();
	//Renderer::SetShader(SHADER_PATH_LINES);
	//glUseProgram(Renderer::GetShaderProgramID());

	//// Send the view projection constants to the shader
	//VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");
	//glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	//for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
	//{
	//	mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
	//	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	//	(*it)->Draw();
	//}

	//for (vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
	//{
	//	mat4 VP = mCamera[mCurrentCamera]->GetViewProjectionMatrix();
	//	glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

	//	(*it)->Draw();
	//}

    Renderer::CheckForErrors();
    
    // Draw Billboards
    mpBillboardList->Draw();


	// Restore previous shader

	// 2D Text Alex

	//// Display screen texture
	////Icons2D::displayIcon(0, 10, 25, 250); // displays time icon next to time

	//// Print the text in timerText to the screen
	//Text2D::print2DText(timerText, 50, 50, 17);
	//Icons2D::displayIcon(2, 20, 50, 20); // displays time icon next to time

	//// Display score counter
	//// When an enemy is destroyed by one of our particles, call function to increase player score by 1
	//// function to call: IncrementPlayerScore()
	//sprintf(scoreText, "score:%.f", playerScore);
	//Text2D::print2DText(scoreText, 50, 25, 20);
	//Icons2D::displayIcon(3, 20, 25, 20); // displays score icon next to score

	//sprintf(healthText, "HP:", playerScore);
	//Text2D::print2DText(healthText, 50, 75, 20);
	//if(HealthBar::GetHP() > 0 ){
	//	HealthBar::displayHP(95, 75, 20);
	//}

	//Icons2D::displayIcon(1, 20, 75, 20); // displays hp icon next to hp
// 2D Text Alex

	// ALEX MENU display UI components only in certain states*********************************

	// Display menu texture and game title
	if (!gameInProgress && !playerDead)
	{
		Text2D::print2DText(title, 190, 415, 20);
		Icons2D::displayIcon(0, 271, 160, 250); // displays time icon next to time
	}

	// If player's score surpasses 20, and player is still alive-> display LEVEL CLEARED
	if (playerScore > 20.0f && !playerDead)
	{
		Text2D::print2DText(lvlClearText, 220, 475, 30);
	}

	// Print the text in timerText to the screen
	if (gameInProgress)
	{
		Text2D::print2DText(timerText, 50, 25, 20);
		Icons2D::displayIcon(2, 20, 20, 25); // displays time icon next to time
	}
	
	// Display score counter
	// When an enemy is destroyed by one of our particles, call function to increase player score by 1
	// function to call: IncrementPlayerScore()

	// When HP falls to zero, stop showing text for score and HP
	// *** TODO PROJ : actually kill the player ship model and remove all enemies and meteors***
	if (!playerDead)
	{
		sprintf(scoreText, "score:%.f", playerScore);
		Text2D::print2DText(scoreText, 50, 50, 17);
		Icons2D::displayIcon(3, 20, 45, 25); // displays score icon next to score

		sprintf(healthText, "HP ", playerScore);
		Text2D::print2DText(healthText, 50, 75, 20);
		if (HealthBar::GetHP() > 0){
			HealthBar::displayHP(95, 75, 20);
		}
		Icons2D::displayIcon(1, 20, 72, 25); // displays hp icon next to hp
	}
	else if (playerDead)
	{
		// player has died, display GAME OVER
		Text2D::print2DText(gameOverText, 265, 430, 30);
		sprintf(scoreText, "final score:%.f", playerScore);
		Text2D::print2DText(scoreText, 262, 400, 22);
	}

	// ALEX MENU *********************************

	// 2D Text Alex
	//Renderer::SetShader((ShaderType) prevShader);

	//glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	Renderer::CheckForErrors();
		glViewport(0,0,1024,768); // Render on the whole framebuffer, complete from the lower left corner to the upper right
		Renderer::CheckForErrors();
		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Renderer::CheckForErrors();
		// Use our shader
		glUseProgram(quad_programID);
		Renderer::CheckForErrors();
		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
		Renderer::CheckForErrors();
		// Set our "renderedTexture" sampler to user Texture Unit 0
		glUniform1i(texID, 0);
		Renderer::CheckForErrors();
		//glUniform1f(timeID, (float)(glfwGetTime()*10.0f) );
		GLuint shockCenter = glGetUniformLocation(quad_programID, "shockCenter");
		const Camera* cCamera = GetCurrentCamera();
		
		glm::vec4 shipOnScreen4 = (cCamera->GetViewProjectionMatrix()*(vec4(mShipModel->GetPosition(),1)));
		glm::vec2 shipOnScreen2 = vec2(shipOnScreen4.x/shipOnScreen4.w, shipOnScreen4.y/shipOnScreen4.w);
		shipOnScreen2.x = (shipOnScreen2.x + 1.0f)*0.5f;
		shipOnScreen2.y = (shipOnScreen2.y + 1.0f)*0.5f;
		//std::cout << shipOnScreen2.x<<endl;
		//std::cout << shipOnScreen2.y<<endl<<endl;
		//std::cout << shockTime;
		glUniform2f(shockCenter, shipOnScreen2.x,shipOnScreen2.y);
		GLuint shockTimeID = glGetUniformLocation(quad_programID, "shockTime");
		glUniform1f(shockTimeID, shockTime );
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		Renderer::CheckForErrors();
		glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
		Renderer::CheckForErrors();
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		Renderer::CheckForErrors();
		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles
		Renderer::CheckForErrors();
		glDisableVertexAttribArray(0);
		glEnable(GL_DEPTH_TEST);
	Renderer::EndFrame();
}

void World::LoadScene(const char * scene_path)
{
	// Using case-insensitive strings and streams for easier parsing
	ci_ifstream input;
	input.open(scene_path, ios::in);

	// Invalid file
	if(input.fail() )
	{	 
		fprintf(stderr, "Error loading file: %s\n", scene_path);
		getchar();
		exit(-1);
	}

	ci_string item;
	while( std::getline( input, item, '[' ) )   
	{
        ci_istringstream iss( item );

		ci_string result;
		if( std::getline( iss, result, ']') )
		{
			if( result == "cube" )
			{
				// Box attributes
				CubeModel* cube = new CubeModel();
				cube->Load(iss);
				mModel.push_back(cube);
			}
            else if( result == "sphere" )
            {
                SphereModel* sphere = new SphereModel();
                sphere->Load(iss);
                mModel.push_back(sphere);
            }
			else if ( result == "animationkey" )
			{
				AnimationKey* key = new AnimationKey();
				key->Load(iss);
				mAnimationKey.push_back(key);
			}
			else if (result == "animation")
			{
				Animation* anim = new Animation();
				anim->Load(iss);
				mAnimation.push_back(anim);
			}
			else if ( result.empty() == false && result[0] == '#')
			{
				// this is a comment line
			}else if(result == "cModel")
			{
				
				ShipModel* ship = new ShipModel(shipTextureID);
				ship->Load(iss);
				mModel.push_back(ship);
				mShipModel = ship;
			}else if(result == "eModel")
			{
				

				//TINO set X borders for ennemies
				float lBound = -10.0f;
				float uBound = 10.0f;

				//TINO position
				//positionX will be generated randomly between -5 and 5
				float ennemyPositionX = lBound + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(uBound-lBound)));
				
				//TINO 10 ennemies
				ShipEnnemyModel* ennemy;
				ennemy = new ShipEnnemyModel(droidTextureID);
				ennemy->Load(iss);
				//float ennemyPositionZ = 10.0f;
				vec3 EnnemyPosition = vec3(ennemyPositionX, 0.0f, ennemy->GetPosition().z);//just modify x and z
				ennemy->SetPosition(EnnemyPosition);
				mShipEnnemyModel.push_back(ennemy);


				////TINO set up the ennemis
				//for(int i=1; i<10;i++){
				//	ennemy[i] = new ShipEnnemyModel(droidTextureID);
				//	ennemy[i]->Load(iss);
					
				//	//vec3 EnnemySize = vec3(0.1f, 0.1f, 0.1f);
				//	ennemy[i]->SetPosition(EnnemyPosition);
				//	ennemy[i]->SetScaling(EnnemySize);
				//	mModel.push_back(ennemy[i]);

				//	ennemyPositionZ += 5.0f;//next ennemy will be 10 step further in Z

				//	//update X random position for next ennemy
				//	ennemyPositionX = lBound + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(uBound-lBound)));
				//}


				//TINO
				/*ShipEnnemyModel* chair2 = new ShipEnnemyModel(shipTextureID);
				chair2->Load(iss);
				vec3 EnnemyPosition = vec3(0.0f, 0.0f, 10.0f);//just modify x and z
				vec3 EnnemySize = vec3(0.1f, 0.1f, 0.1f);
				chair2->SetPosition(EnnemyPosition);
				chair2->SetScaling(EnnemySize);
				mModel.push_back(chair2);*/

			}else if(result == "Meteor")
			{
				

				//TINO set X borders for ennemies
				float lBound = -6.0f;
				float uBound = 6.0f;

				//TINO position
				//positionX will be generated randomly between -5 and 5
				float MeteorPositionX = lBound + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(uBound-lBound)));
				float MeteorPositionZ = 15.0f;

				//TINO 10 ennemies
				MeteorModel* Meteor[3];

				//TINO set up the ennemis

				Meteor[0] = new MeteorModel(MeteorTextureID);
				Meteor[0]->Load(iss);
				vec3 MeteorSize = Meteor[0]->GetScaling();
				for(int i=1; i<3;i++){
					Meteor[i] = new MeteorModel(MeteorTextureID);
					Meteor[i]->Load(iss);
					vec3 MeteorPosition = vec3(MeteorPositionX, 0.0f, MeteorPositionZ);//just modify x and z
					//vec3 EnnemySize = vec3(0.1f, 0.1f, 0.1f);
					Meteor[i]->SetPosition(MeteorPosition);
					Meteor[i]->SetScaling(MeteorSize);
					mModel.push_back(Meteor[i]);

					MeteorPositionZ += 5.0f;//next ennemy will be 10 step further in Z

					//update X random position for next ennemy
					MeteorPositionX = lBound + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(uBound-lBound)));
				}
			}else if(result == "pModel")
			{	
				Projectile* proj = new Projectile(projTextureID);
				proj->Load(iss);
				mProjectile.push_back(proj);
			}	else if(result == "Heart"){
				HeartModel* heart = new HeartModel(heartTextureID);
				heart->Load(iss);
				mHeartModel.push_back(heart);
			}

			else if(result == "mpModel")
			{	
				PlayerProjectile* proj = new PlayerProjectile(projTextureID);
				proj->Load(iss);
				mPlayerProjectile.push_back(proj);

				ParticleDescriptor* fireProjectile;//Tino2
				ParticleSystem* ps_projectile;//Tino2
				ParticleEmitter* proj_emitter;//Tino2


				fireProjectile = new ParticleDescriptor();//Tino2
				fireProjectile->SetFireProjectile();//Tino2
				proj_emitter = new ParticleEmitter(vec3(0.0f,0.0f,-100.0f));//Tino2
				ps_projectile = new ParticleSystem(proj_emitter, fireProjectile);//Tino2
				AddParticleSystem(ps_projectile);//Tino2
				projEmmitter.push_back(proj_emitter);//Tino2
			}
			else if(result == "Earth"){

				//EarthModel* earth = new EarthModel(earthTextureID);
				PlanetModel* earth = new PlanetModel(earthTextureID, true, false, vec4(0.4f, 1.0f, 0.5f, 90.0f));
				earth->Load(iss);
				mModel.push_back(earth);


			}
			else if(result == "Moon"){
				PlanetModel* moon = new PlanetModel(moonTextureID, true, true, vec4(0.4f, 0.6f, 0.2f, 90.0f));
				moon->Load(iss);
				mModel.push_back(moon);


			}
			else if (result == "BackgroundSphere"){
				//BackgroundSphere* background = new BackgroundSphere(spaceTextureID);
				PlanetModel* background = new PlanetModel(spaceTextureID, true, false, vec4(0.5f, 0.8f, 0.0f, 90.0f));
				background->Load(iss);
				mModel.push_back(background);


			}
			else if (result == "Sun"){
				//Loading the sun should set the light at the same position as the sun
				//SunModel* sun = new SunModel(sunTextureID);
				PlanetModel* sun = new PlanetModel(sunTextureID, true, true, vec4(1.5f, 0.8f, 0.2f, 90.0f));
				sun->Load(iss);
				lightPosition = vec4(sun->GetPosition(),0);
				mModel.push_back(sun);
				

			}
			else if (result == "Mars"){
				//MarsModel* mars = new MarsModel(marsTextureID);
				PlanetModel* mars = new PlanetModel(marsTextureID, true, false, vec4(0.4f, 1.0f, 0.2f, 90.0f));
				mars->Load(iss);
				mModel.push_back(mars);
			}

			else
			{
				fprintf(stderr, "Error loading scene file... !");
				getchar();
				exit(-1);
			}
	    }
	}
	input.close();

	// Set Animation vertex buffers
	for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
	{
		// Draw model
		(*it)->CreateVertexBuffer();
	}
}


Animation* World::FindAnimation(ci_string animName)
{
    for(std::vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
    {
        if((*it)->GetName() == animName)
        {
            return *it;
        }
    }
    return nullptr;
}

AnimationKey* World::FindAnimationKey(ci_string keyName)
{
    for(std::vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
    {
        if((*it)->GetName() == keyName)
        {
            return *it;
        }
    }
    return nullptr;
}

const Camera* World::GetCurrentCamera() const
{
     return mCamera[mCurrentCamera];
}

void World::AddBillboard(Billboard* b)
{
    mpBillboardList->AddBillboard(b);
}

void World::RemoveBillboard(Billboard* b)
{
    mpBillboardList->RemoveBillboard(b);
}

void World::AddParticleSystem(ParticleSystem* particleSystem)
{
    mParticleSystemList.push_back(particleSystem);
}

void World::RemoveParticleSystem(ParticleSystem* particleSystem)
{
    vector<ParticleSystem*>::iterator it = std::find(mParticleSystemList.begin(), mParticleSystemList.end(), particleSystem);
    mParticleSystemList.erase(it);
}
Animation* World::GetmAnimation(int i)
{
	return World::mAnimation[i];
}
void World::LoadNextProjectile(){
nextProjectile++;
	if (nextProjectile >= 15){
		nextProjectile-= 15;
	}
}
void World::LoadNextHeart(){
	nextHeart+=1;
	if (nextHeart >= 15){
		nextHeart-= 15;
	}
}
void World::ResetSpawnTime(){
	spawntime = 0.0f;
}

// 2D Text Alex

void World::IncrementPlayerScore()
{
	playerScore = playerScore + 1.0f;

	// if (playerScore > ? go to new scene (Mars) and reset player score and time
}

// 2D Text Alex
