//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once


#include "ParsingHelper.h"
#include "Billboard.h"
#include <vector>
#include "sceneLoader.h"
#include "Projectile.h"
#include "PlayerProjectile.h"
#include "ShipEnnemyModel.h"
#include "ShipModel.h"
#include "HeartModel.h"
#include "ParticleEmitter.h"//Tino
#include "MeteorModel.h"//Tino

class Camera;
class Model;
class Animation;
class AnimationKey;
class ParticleSystem;


class World
{
public:
	World(int level);
	~World();
	
	unsigned int currentLevel;
    static World* GetInstance();

	unsigned int nextProjectile;
	unsigned int nextHeart;

	//Texture IDs

	int droidTextureID;// = TextureLoader::LoadTexture("../Assets/Textures/droid.tga");
	int MeteorTextureID;// = TextureLoader::LoadTexture("../Assets/Textures/meteor.jpg");
	int shipTextureID;// = TextureLoader::LoadTexture("../Assets/Textures/ship1.jpg");
	int spaceTextureID;
	int projTextureID;
	int earthTextureID;
	int moonTextureID;
	int sunTextureID;
	int marsTextureID;
	int heartTextureID;
	ParticleEmitter* emitter_ennemy;//Tino made global
	ShipModel* ship;//Tino made global
	ParticleEmitter* emitter;//Tino made global
	ShipEnnemyModel* ennemy;//Tino made it global
	MeteorModel* Meteor[3];//Tino made it global

	ParticleEmitter* emitter_damage_2;//Tino2 made global
	ParticleEmitter* emitter_damage_3;//Tino2 made global
	ParticleEmitter* emitter_projectile;//Tino2 made global

	//Models

	sceneLoader* meteorScene;
	sceneLoader* droidScene;
	sceneLoader* projScene;
	sceneLoader* planetScene;
	sceneLoader* heartScene;
	void Update(float dt);
	void Draw();

	void LoadScene(const char * scene_path);
    Animation* FindAnimation(ci_string animName);
	AnimationKey* FindAnimationKey(ci_string keyName);

    const Camera* GetCurrentCamera() const;
    void AddBillboard(Billboard* b);
    void RemoveBillboard(Billboard* b);
    void AddParticleSystem(ParticleSystem* particleSystem);
    void RemoveParticleSystem(ParticleSystem* particleSystem);

	
	glm::vec3 lightColor;// = vec3(1.0f, 1.0f, 1.0f);
	float lightKc;// = 0.0f;
	float lightKl;//= 0.0f;
	float lightKq;//  = 1.0f;
	glm::vec4 lightPosition;// = vec4(2.0f, 2.0f, -2.0f, 1.0f);
	// Material Coefficients
	float ka ;
	float kd ;
	float ks ;
	float n ;
	float spawntime;
	std::vector<Model*> mModel;
	std::vector<Projectile*> mProjectile;
	std::vector<ParticleEmitter*> projEmmitter;//Tino
	std::vector<PlayerProjectile*> mPlayerProjectile;
	std::vector<ShipEnnemyModel*> mShipEnnemyModel;
	std::vector<HeartModel*> mHeartModel;
	ShipModel* mShipModel; 
	Animation* GetmAnimation(int i);
	void LoadNextProjectile();
	void LoadNextHeart();
	void ResetSpawnTime();

	//Constants for rotation system

	glm::vec3 rotationAxis;

	float orbitRotationConstantNumerator;
	float orbitRotationConstantDenominator;

	float stretchConstant;

	float selfRotationConstant;

//MAKS moved playerscore from .cpp to .h
	float playerScore;

	bool lvlDone; //MAKS added bool to check for level end

	// ALEX MENU ******************************************
	bool gameInProgress;
	bool playerDead;

	void startGame();
	// ALEX MENU ******************************************

	void moveShiptailFlameDead();
	void IncrementPlayerScore();




	GLuint FramebufferName;
	GLuint renderedTexture;
	GLuint depthrenderbuffer;
	GLenum DrawBuffers;
	GLuint quad_vertexbuffer;
	GLuint texID;
	GLuint quad_programID;
	float shockTime;
    
private:
    static World* instance;
    
	
    std::vector<Animation*> mAnimation;
    std::vector<AnimationKey*> mAnimationKey;
	std::vector<Camera*> mCamera;
    
	unsigned int mCurrentCamera;
	std::vector<ParticleSystem*> mParticleSystemList;
    BillboardList* mpBillboardList;
};
//const glm::vec4 lightPosition(2.0f, 2.0f, -2.0f, 1.0f);
//const glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
