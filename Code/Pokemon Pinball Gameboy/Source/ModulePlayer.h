#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

class PhysBody;
class PhysicEntity;
class PlayerSpring;

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	Texture2D playerImg;
	Texture2D colisionImg;
	Texture2D playerBodyImg;

	PlayerSpring* player = nullptr;
	PlayerSpring* playerColision = nullptr;
	PlayerSpring* playerBody = nullptr;

	//PinballFlipper* leftFlipper = nullptr;
	//PinballFlipper* rightFlipper = nullptr;

	float posX = 10.0;
	float posY = 12.0;

	bool canShoot = true;
	bool isShooting = false;

	float t = 0.0;
};