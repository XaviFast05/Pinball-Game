#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

class PlayerSpring : public PhysicEntity
{
public:

	PlayerSpring(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateRectangle(_x, _y, 37, 100), _listener)
		, texture(_texture)
	{

	}

	virtual ~PlayerSpring() = default;

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		float scale = 3;
		DrawTexturePro(texture, Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
			Rectangle{ (float)x, (float)y, (float)texture.width * scale, (float)texture.height * scale },
			Vector2{ (float)texture.width / 0.7f, (float)texture.height / 0.7f }, body->GetRotation() * RAD2DEG, WHITE);
	}

	void UpdatePlayer(float x, float y)
	{
		this->body->body->SetTransform(b2Vec2{ x, y }, 0);
		float scale = 3;
		
	}

	virtual int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal)
	{
		return 0;
	}

	Texture2D texture;
};

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	playerImg = LoadTexture("Assets/Diglet.png");
	player = new PlayerSpring(App->physics, 500, 800, this, playerImg);


	LOG("Loading player");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	//player->UpdatePlayer(posX, posY);

	if (IsKeyPressed(KEY_W) && posY > 6.0 && canShoot)
	{
		player->body->body->SetLinearVelocity(b2Vec2{ 0.0 , 1.0 });
		if (player->body->body->GetPosition().y <= 7.0)
		{
			canShoot = false;
		}
		t += 0.2;
	}

	if (IsKeyReleased(KEY_W) && posY > 6.0 && canShoot)
	{
		isShooting = true;
	}

	if (isShooting)
	{
		player->body->body->SetLinearVelocity(b2Vec2{ 0.0 , -12.0 -t });
	}

	if (player->body->body->GetPosition().y < 15.5)
	{
		posY = player->body->body->GetPosition().y;
		player->UpdatePlayer(posX, posY);
		posY += 0.5;
		canShoot = false;
		isShooting = false;
	}

	if (posY >= 15.5)
	{
		canShoot = true;
	}

	return UPDATE_CONTINUE;
}