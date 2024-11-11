#pragma once

#include "Globals.h"
#include "Module.h"

#include "p2Point.h"

#include "raylib.h"
#include <vector>

class PhysBody;
class Circle;
class PhysicEntity;

class ModuleGame : public Module
{
public:
	ModuleGame(Application* app, bool start_enabled = true);
	~ModuleGame();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void ScoreRefresh();
public:

	std::vector<PhysicEntity*> entities;
	
	PhysBody* sensor;
	
	bool sensed;
	bool isStopped = false;

	//Voltorb* BodyVoltorb;

	Texture2D circle;
	Texture2D box;
	Texture2D voltorb;
	Texture2D square;

	Texture2D background;
	Texture2D wallTexture;
	Texture2D wallTexture2;
	Texture2D wallTexture3;
	Texture2D wallTexture4;
	Texture2D wallTexture5;
	Texture2D wallTexture6;
	Texture2D wallTexture7;
	Texture2D wallTexture8;
	Texture2D wallTexture9;
	Texture2D wallTexture10;
	Texture2D wallTexture11;

	uint32 bonus_fx;

	vec2<int> ray;
	bool ray_on;
	Music backgroundMusic;
	int score = 0;
	int record = 0;
	int pastScore = 0;
	int lifes = 3;

	Circle* Pokeball;
};