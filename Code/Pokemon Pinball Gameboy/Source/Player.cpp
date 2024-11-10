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

	PlayerSpring(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture, int w, int h)
		: PhysicEntity(physics->CreateRectangle(_x, _y, w, h), _listener)
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
	LOG("Loading player");
	playerImg = LoadTexture("Assets/Diglet.png");
	playerBodyImg = LoadTexture("Assets/DigletBody.png");

	player = new PlayerSpring(App->physics, 500, 800, this, playerImg, 41, 50);
	playerBody = new PlayerSpring(App->physics, 500, 800, this, playerBodyImg, 41, 230);
	playerColision = new PlayerSpring(App->physics, 500, 800, this, colisionImg, 37, 260);

	playerBody->body->body->SetEnabled(false);
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
	playerBody->UpdatePlayer(posX, player->body->body->GetPosition().y + 2.85);

	if (IsKeyReleased(KEY_W) && canShoot)
	{
		isShooting = true;
		canShoot = false;
	}

	if (!isShooting && !IsKeyDown(KEY_W))
	{
		player->UpdatePlayer(posX, posY);
		
		canShoot = true;
		t = 0;
	}

	if (IsKeyPressed(KEY_W) && canShoot)
	{
		player->body->body->SetLinearVelocity(b2Vec2{ 0.0 , 0.5 });
		playerColision->UpdatePlayer(-10, 13.0);
	}

	if (IsKeyPressedRepeat(KEY_W) && canShoot && t < 16)
	{
		t += 2;
	}

	if (player->body->body->GetPosition().y < 12.0 && isShooting)
	{
		playerColision->UpdatePlayer(posX, 13.0);
	}

	if (player->body->body->GetPosition().y < 12.0)
	{
		isShooting = false;
		player->UpdatePlayer(posX, posY);
	}

	if (isShooting)
	{
		player->body->body->SetLinearVelocity(b2Vec2{ 0.0 ,  (float)(-12.2 - t) });
	}

	player->Update();

	return UPDATE_CONTINUE;
}