//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//


#include "Renderer.h"
#include "World.h"
#include "EventManager.h"
#include "Billboard.h"
#include "TextureLoader.h"
#include "Text2D.h"
#include "HealthBar.h"

#include <GLFW/glfw3.h>


int main(int argc, char*argv[])
{

	int level = 2;
	while (level < 4){ // MAKS Change level limit to 4
		
		if(level == 3) { level = 0; }

		EventManager::Initialize();
		Renderer::Initialize();

		World world = World(level);    

		if (argc > 1)
		{
			world.LoadScene(argv[1]);
		}
		else
		{
			// TODO - You can alternate between different scenes for testing different things
			// Static Scene contains no animation
			// Animated Scene does
#if defined(PLATFORM_OSX)		
			world.LoadScene("Scenes/AnimatedSceneWithParticles.scene");
			//		world.LoadScene("Scenes/AnimatedScene.scene");
			//		world.LoadScene("Scenes/StaticScene.scene");
			//		world.LoadScene("Scenes/CoordinateSystem.scene");
#else
		//MAKS CHANGE changed the level loading system
		if (level == 0) { world.LoadScene("../Assets/Scenes/AnimatedSceneWithParticles.scene"); }
		if (level == 1) { world.LoadScene("../Assets/Scenes/EarthAnimatedSceneWithParticles.scene"); }
		if (level == 2) {world.LoadScene("../Assets/Scenes/MarsAnimatedSceneWithParticles.scene"); }
		if (level == 3) {world.LoadScene("../Assets/Scenes/MoonAnimatedSceneWithParticles.scene"); }
			//		world.LoadScene("../Assets/Scenes/AnimatedScene.scene");
			//		world.LoadScene("../Assets/Scenes/StaticScene.scene");
			//		world.LoadScene("../Assets/Scenes/CoordinateSystem.scene");
#endif
		}

		// Main Loop


		do
		{
			// Update Event Manager - Frame time / input / events processing 
			EventManager::Update();

			// Update World
			float dt = EventManager::GetFrameTime();

			//Different level testing

		//MAKS CHANGE start --------------------------------------

		if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_R) == GLFW_PRESS)
		{
			break;
		}
			
		if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_L) == GLFW_PRESS)
		{
			level += 1;
			break;
		} else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_K) == GLFW_PRESS)
		{
			if(level != 0){
				level -= 1;

				break;
			}
		}  

		if(World::GetInstance()->lvlDone == true){
			level += 1;
			break;
		}

		//MAKS CHANGE end --------------------------------------

			world.Update(dt);

			// Draw World
			world.Draw();        
		}
		while(EventManager::ExitRequested() == false);

		Renderer::Shutdown();
		EventManager::Shutdown();
		Text2D::clean2DText();
		HealthBar::cleanUpHP();
		
	}
	return 0;
}
