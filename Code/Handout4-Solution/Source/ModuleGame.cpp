#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleGame.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

class PhysicEntity
{
protected:

	PhysicEntity(PhysBody* _body, Module* _listener)
		: body(_body)
		, listener(_listener)
	{
		body->listener = listener;
	}

public:
	virtual ~PhysicEntity() = default;
	virtual void Update() = 0;

	virtual int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal)
	{
		return 0;
	}

protected:
	PhysBody* body;
	Module* listener;
};

class Circle : public PhysicEntity
{
public:
	Circle(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateCircle(_x, _y, 20), _listener)
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		Vector2 position{ (float)x, (float)y };
		float scale = 3.0f;
		Rectangle source = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
		Rectangle dest = { position.x , position.y , (float)texture.width * scale, (float)texture.height * scale };
		Vector2 origin = { (float)texture.width * 1.5f , (float)texture.height * 1.5f };
		float rotation = body->GetRotation() * RAD2DEG;
		DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
	}

private:
	Texture2D texture;

};

class Box : public PhysicEntity
{
public:
	Box(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateRectangle(_x, _y, 100, 50), _listener)
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTexturePro(texture, Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
			Rectangle{ (float)x, (float)y, (float)texture.width, (float)texture.height },
			Vector2{ (float)texture.width / 2.0f, (float)texture.height / 2.0f}, body->GetRotation() * RAD2DEG, WHITE);
	}

	int RayHit(vec2<int> ray, vec2<int> mouse, vec2<float>& normal) override
	{
		return body->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);;
	}

private:
	Texture2D texture;

};

class Wall : public PhysicEntity
{
public:
	// Pivot 0, 0
	static constexpr int wall_points[104] = {
			104 * SCREEN_SIZE, 277 * SCREEN_SIZE,
			137 * SCREEN_SIZE, 256 * SCREEN_SIZE,
			152 * SCREEN_SIZE, 246 * SCREEN_SIZE,
			152 * SCREEN_SIZE, 206 * SCREEN_SIZE,
			150 * SCREEN_SIZE, 200 * SCREEN_SIZE,
			144 * SCREEN_SIZE, 195 * SCREEN_SIZE,
			136 * SCREEN_SIZE, 194 * SCREEN_SIZE,
			136 * SCREEN_SIZE, 174 * SCREEN_SIZE,
			131 * SCREEN_SIZE, 166 * SCREEN_SIZE,
			134 * SCREEN_SIZE, 161 * SCREEN_SIZE,
			141 * SCREEN_SIZE, 148 * SCREEN_SIZE,
			145 * SCREEN_SIZE, 131 * SCREEN_SIZE,
			148 * SCREEN_SIZE, 118 * SCREEN_SIZE,
			150 * SCREEN_SIZE, 101 * SCREEN_SIZE,
			150 * SCREEN_SIZE, 72 * SCREEN_SIZE,
			147 * SCREEN_SIZE, 58 * SCREEN_SIZE,
			139 * SCREEN_SIZE, 40 * SCREEN_SIZE,
			149 * SCREEN_SIZE, 54 * SCREEN_SIZE,
			155 * SCREEN_SIZE, 65 * SCREEN_SIZE,
			160 * SCREEN_SIZE, 83 * SCREEN_SIZE,
			160 * SCREEN_SIZE, 278 * SCREEN_SIZE,
			174 * SCREEN_SIZE, 278 * SCREEN_SIZE,
			173 * SCREEN_SIZE, 84 * SCREEN_SIZE,
			170 * SCREEN_SIZE, 72 * SCREEN_SIZE,
			167 * SCREEN_SIZE, 60 * SCREEN_SIZE,
			158 * SCREEN_SIZE, 45 * SCREEN_SIZE,
			149 * SCREEN_SIZE, 34 * SCREEN_SIZE,
			137 * SCREEN_SIZE, 24 * SCREEN_SIZE,
			122 * SCREEN_SIZE, 16 * SCREEN_SIZE,
			99 * SCREEN_SIZE, 9 * SCREEN_SIZE,
			71 * SCREEN_SIZE, 9 * SCREEN_SIZE,
			64 * SCREEN_SIZE, 11 * SCREEN_SIZE,
			39 * SCREEN_SIZE, 22 * SCREEN_SIZE,
			33 * SCREEN_SIZE, 26 * SCREEN_SIZE,
			29 * SCREEN_SIZE, 30 * SCREEN_SIZE,
			25 * SCREEN_SIZE, 30 * SCREEN_SIZE,
			25 * SCREEN_SIZE, 23 * SCREEN_SIZE,
			9 * SCREEN_SIZE, 23 * SCREEN_SIZE,
			9 * SCREEN_SIZE, 100 * SCREEN_SIZE,
			11 * SCREEN_SIZE, 110 * SCREEN_SIZE,
			14 * SCREEN_SIZE, 128 * SCREEN_SIZE,
			18 * SCREEN_SIZE, 143 * SCREEN_SIZE,
			22 * SCREEN_SIZE, 157 * SCREEN_SIZE,
			28 * SCREEN_SIZE, 166 * SCREEN_SIZE,
			22 * SCREEN_SIZE, 174 * SCREEN_SIZE,
			23 * SCREEN_SIZE, 193 * SCREEN_SIZE,
			16 * SCREEN_SIZE, 194 * SCREEN_SIZE,
			10 * SCREEN_SIZE, 199 * SCREEN_SIZE,
			7 * SCREEN_SIZE, 206 * SCREEN_SIZE,
			8 * SCREEN_SIZE, 246 * SCREEN_SIZE,
			22 * SCREEN_SIZE, 254 * SCREEN_SIZE,
			56 * SCREEN_SIZE, 277 * SCREEN_SIZE
	



	};

	Wall(ModulePhysics* physics, int _x, int _y, Module* _listener, Texture2D _texture)
		: PhysicEntity(physics->CreateChain(0, 0, wall_points , 104), _listener)
		, texture(_texture)
	{

	}

	void Update() override
	{
		int x, y;
		body->GetPhysicPosition(x, y);
		DrawTextureEx(texture, Vector2{ (float)x, (float)y }, body->GetRotation() * RAD2DEG, 1.0f, WHITE);
	}

private:
	Texture2D texture;
};



ModuleGame::ModuleGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	ray_on = false;
	sensed = false;
}

ModuleGame::~ModuleGame()
{}

// Load assets
bool ModuleGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = LoadTexture("Assets/wheel.png"); 
	box = LoadTexture("Assets/crate.png");
	rick = LoadTexture("Assets/rick_head.png");
	background = LoadTexture("Assets/Fondo.png");
	wallTexture = LoadTexture("Assets/wall_texture.png");
	
	bonus_fx = App->audio->LoadFx("Assets/bonus.wav");

	//sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	entities.emplace_back(new Wall(App->physics, SCREEN_SIZE, SCREEN_SIZE, this, wallTexture));

	return ret;
}

// Load assets
bool ModuleGame::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleGame::Update()
{
	float scaleX = (float)GetScreenWidth() / background.width;
	float scaleY = (float)GetScreenHeight() / background.height;
	DrawTextureEx(background, { 0, 0 }, 0.0f, fmax(scaleX, scaleY), WHITE);




	if(IsKeyPressed(KEY_SPACE))
	{
		ray_on = !ray_on;
		ray.x = GetMouseX();
		ray.y = GetMouseY();
	}

	if(IsKeyPressed(KEY_ONE))
	{
		entities.emplace_back(new Circle(App->physics, GetMouseX(), GetMouseY(), this, circle));
		
	}

	if(IsKeyPressed(KEY_TWO))
	{
		entities.emplace_back(new Box(App->physics, GetMouseX(), GetMouseY(), this, box));
	}





	// Prepare for raycast ------------------------------------------------------
	
	vec2i mouse;
	mouse.x = GetMouseX();
	mouse.y = GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	vec2f normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------


	for (PhysicEntity* entity : entities)
	{
		entity->Update();
		if (ray_on)
		{
			int hit = entity->RayHit(ray, mouse, normal);
			if (hit >= 0)
			{
				ray_hit = hit;
			}
		}
	}
	
	if (IsKeyPressed(KEY_SPACE))
	{
		ray_on = !ray_on;
		ray.x = GetMouseX();
		ray.y = GetMouseY();
	}

	// ray -----------------
	if(ray_on == true)
	{
		vec2f destination((float)(mouse.x-ray.x), (float)(mouse.y-ray.y));
		destination.Normalize();
		destination *= (float)ray_hit;

		DrawLine(ray.x, ray.y, (int)(ray.x + destination.x), (int)(ray.y + destination.y), RED);

		if (normal.x != 0.0f)
		{
			DrawLine((int)(ray.x + destination.x), (int)(ray.y + destination.y), (int)(ray.x + destination.x + normal.x * 25.0f), (int)(ray.y + destination.y + normal.y * 25.0f), Color{ 100, 255, 100, 255 });
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	App->audio->PlayFx(bonus_fx);
}
