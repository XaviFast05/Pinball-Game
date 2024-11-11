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
class PinballFlipperR;
class PinballFlipperL;

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
	Texture2D Lflipper;
	Texture2D Rflipper;

	PlayerSpring* player = nullptr;
	PlayerSpring* playerColision = nullptr;
	PlayerSpring* playerBody = nullptr;

	PinballFlipperL* leftFlipper = nullptr;
	PinballFlipperR* rightFlipper = nullptr;

	uint32 FlipperUp_fx;
	uint32 FlipperDown_fx;

	float posX = 10.0;
	float posY = 12.0;

	bool canShoot = true;
	bool isShooting = false;
	bool flipperDownL = false;
	bool flipperDownR = false;

	float t = 0.0;
};