#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "PhysicEntity.h"

class PinballFlipperR : public PhysicEntity
{
public:
	b2RevoluteJoint* joint; // Joint para la palanca

	PinballFlipperR(ModulePhysics* physics, int _x, int _y, int width, int height, Texture2D _texture, Module* _listener)
		: PhysicEntity(physics->CreateRectangle(_x, _y, width, height), _listener), joint(nullptr)
		, texture(_texture)
	{

		// Crear el punto de anclaje de la palanca
		PhysBody* anchorPhysBody = physics->CreatePoint(_x, _y);
		b2Body* anchor = anchorPhysBody->body;

		// Configurar el joint para la palanca
		b2RevoluteJointDef jointDef;
		jointDef.bodyA = anchor;
		jointDef.bodyB = body->body;
		jointDef.localAnchorA.Set(0, 0);  // Ancla en el centro del punto de fijación
		jointDef.localAnchorB.Set(0.5f, 0);  // Ancla en el centro de la palanca
		jointDef.enableLimit = true;
		jointDef.lowerAngle = -30 * b2_pi / 180;  // Ángulo mínimo
		jointDef.upperAngle = 30 * b2_pi / 180;   // Ángulo máximo
		jointDef.maxMotorTorque = 1000.0f;
		jointDef.motorSpeed = 0.0f;
		jointDef.enableMotor = true;

		joint = (b2RevoluteJoint*)physics->GetWorld()->CreateJoint(&jointDef);
	}

	void Activate() {
		joint->SetMotorSpeed(20.0f); // Velocidad para mover la palanca hacia arriba
	}

	void Deactivate() {
		joint->SetMotorSpeed(-10.0f); // Velocidad para devolver la palanca a su posición original
	}

	void Update() override {
		int x, y;
		body->GetPhysicPosition(x, y);
		float scale = 3;
		DrawTexturePro(texture, Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
			Rectangle{ (float)x, (float)y, (float)texture.width * scale, (float)texture.height * scale },
			Vector2{ (float)texture.width / 0.7f, (float)texture.height / 0.8f }, body->GetRotation() * RAD2DEG, WHITE);
	}
	Texture2D texture;
};

class PinballFlipperL : public PhysicEntity
{
public:
	b2RevoluteJoint* joint; // Joint para la palanca

	PinballFlipperL(ModulePhysics* physics, int _x, int _y, int width, int height, Texture2D _texture, Module* _listener)
		: PhysicEntity(physics->CreateRectangle(_x, _y, width, height), _listener), joint(nullptr)
		, texture(_texture)
	{

		// Crear el punto de anclaje de la palanca
		PhysBody* anchorPhysBody = physics->CreatePoint(_x, _y);
		b2Body* anchor = anchorPhysBody->body;

		// Configurar el joint para la palanca
		b2RevoluteJointDef jointDef;
		jointDef.bodyA = anchor;
		jointDef.bodyB = body->body;
		jointDef.localAnchorA.Set(0, 0);  // Ancla en el centro del punto de fijación
		jointDef.localAnchorB.Set(-0.5f, 0);  // Ancla en el centro de la palanca
		jointDef.enableLimit = true;
		jointDef.lowerAngle = -30 * b2_pi / 180;  // Ángulo mínimo
		jointDef.upperAngle = 30 * b2_pi / 180;   // Ángulo máximo
		jointDef.maxMotorTorque = 1000.0f;
		jointDef.motorSpeed = 0.0f;
		jointDef.enableMotor = true;

		joint = (b2RevoluteJoint*)physics->GetWorld()->CreateJoint(&jointDef);
	}

	void Activate() {
		joint->SetMotorSpeed(-20.0f); // Velocidad para mover la palanca hacia arriba
	}

	void Deactivate() {
		joint->SetMotorSpeed(10.0f); // Velocidad para devolver la palanca a su posición original
	}

	void Update() override {
		int x, y;
		body->GetPhysicPosition(x, y);
		float scale = 3;
		DrawTexturePro(texture, Rectangle{ 0, 0, (float)texture.width, (float)texture.height },
			Rectangle{ (float)x, (float)y, (float)texture.width * scale, (float)texture.height * scale },
			Vector2{ (float)texture.width / 0.6f, (float)texture.height / 0.8f }, body->GetRotation() * RAD2DEG, WHITE);
	}
	Texture2D texture;
};

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
			Vector2{ (float)texture.width / 0.7f, (float)texture.height / 5.3f }, body->GetRotation() * RAD2DEG, WHITE);
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
	Lflipper = LoadTexture("Assets/Palanca_L.png");
	Rflipper = LoadTexture("Assets/Palanca_R.png");

	
	player = new PlayerSpring(App->physics, 500, 800, this, playerImg, 41, 50);
	playerBody = new PlayerSpring(App->physics, 500, 800, this, playerBodyImg, 41, 230);
	playerColision = new PlayerSpring(App->physics, 500, 800, this, colisionImg, 37, 260);
	leftFlipper = new PinballFlipperL(App->physics, 173, 775, 70, 20, Lflipper, this);  
	rightFlipper = new PinballFlipperR(App->physics, 310, 775, 70, 20, Rflipper, this); 

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

	if (IsKeyDown(KEY_A)) {
		leftFlipper->Activate();
	}
	else {
		leftFlipper->Deactivate();
	}

	if (IsKeyDown(KEY_D)) {
		rightFlipper->Activate();
	}
	else {
		rightFlipper->Deactivate();
	}

	// Actualizar las palancas
	leftFlipper->Update();
	rightFlipper->Update();

	player->Update();


	return UPDATE_CONTINUE;
}