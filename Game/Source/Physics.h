#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "Module.h"
#include "Collider.h"
#include "List.h"
#include "Log.h"
#include "Point.h"
#include "DynArray.h"

//#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
//#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !
//
#define PIXELS_PER_METER 1.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 1.0f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

#define DEGREES_TO_RADIANS(angleDegrees) ((angleDegrees) * M_PI / 180.0)
#define RADIANS_TO_DEGREES(angleRadians) ((angleRadians) * 180.0 / M_PI)

#define REALITY_MULTIPLIER 60
#define NEWTONS_MULTIPLIER 100

struct SDL_Texture;
class Collision;
class GuiString;

enum BodyType
{
	NONE = -1,
	STATIC_BODY,
	DYNAMIC_BODY
};

enum CollisionType
{
	UNDEFINED = -1,
	RECTANGLE,
	CIRCLE
};

enum class PhysicsPreset
{
	NO_PRESET = -1,
	PLATFORMER_PHYSICS_PRESET,
	MOON_PHYSICS_PRESET,
	SPACE_PHYSICS_PRESET
};

enum class ScenarioPreset
{
	NO_PRESET = -1,
	LIMITS_1280x720_SCENARIO_PRESET,
	PLATFORMER_1280x720_SCENARIO_PRESET,
	WALLJUMP_1280x720_SCENARIO_PRESET,
	SPHERE_1280x720_SCENARIO_PRESET,
};

enum class Direction
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT
};

enum class DashDirection
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	UPLEFT,
	DOWNLEFT,
	UPRIGHT,
	DOWNRIGHT
};

enum class DebugBools
{
	GROUND,
	ROOF,
	LEFT_WALL,
	RIGHT_WALL,
	JUMP,
	DOUBLE_JUMP,
	WALL_JUMP,
	DASH
};

class Body
{
public:
	Body(BodyType bodyType_, int a) :bodyType(bodyType_), texture(NULL), rect({}), circle({}) {} // Default constructor
	Body(BodyType bodyType_, CollisionType colliderType_ = CollisionType::UNDEFINED, fPoint position_ = { 0.0f,0.0f }, SDL_Rect rect_ = {}, float mass_ = 1.0f) // Constructor with body type and collider type
	{
		bodyType = bodyType_;
		position = position_;
		colliderType = colliderType_;
		rect = rect_;
		mass = mass_;
	}
	Body(BodyType bodyType_, CollisionType colliderType_ = CollisionType::UNDEFINED, fPoint position_ = { 0.0f,0.0f }, CircleCollider circle_ = {}, float mass_ = 1.0f) // Constructor with body type and collider type
	{
		bodyType = bodyType_;
		position = position_;
		colliderType = colliderType_;
		circle = circle_;
		rect = {};
		mass = mass_;
	}

public: //Getters

	// This function returns position x & y of the body
	fPoint GetPosition() const
	{
		return position;
	}
	// This function returns the width & height of the body (Rectangle) or the radius (Circle)
	iPoint GetMagnitudes() const
	{
		if (colliderType == RECTANGLE) return {rect.w, rect.h};
		return {(int)circle.radius, 0};
	}
	// This function returns the type of collision the body has
	CollisionType GetCollisionType() const
	{
		return colliderType;
	}
	// This function returns the type of body it is
	BodyType GetBodyType() const
	{
		return bodyType;
	}
	// This function returns the mass of the body
	float GetMass() const
	{
		return mass;
	}

protected:
	friend class Physics;
	void SolveCollision(Body& body);
	void DeClipper(Body& body);
	double ToPositiveAngle(double angle);

	SDL_Rect rect;
	CircleCollider circle;
	SDL_Texture* texture = nullptr;

	BodyType bodyType;
	CollisionType colliderType;

	fPoint position;
	float rotation;
	float mass;
	bool isCollidable = true;
};

class StaticBody : public Body
{
public:
	StaticBody() :Body(BodyType::STATIC_BODY, 0) {}
	StaticBody(fPoint position_, CollisionType colliderType_, SDL_Rect rect_, uint mass_) :Body(BodyType::STATIC_BODY, colliderType_, position_, rect_, mass_) {}
	StaticBody(fPoint position_, CollisionType colliderType_, CircleCollider circle_, uint mass_) :Body(BodyType::STATIC_BODY, colliderType_, position_, circle_, mass_) {}
};

class DynamicBody : public Body
{
public:
	DynamicBody() :Body(BodyType::DYNAMIC_BODY, 0) {}
	DynamicBody(fPoint position_, fPoint velocity_, fPoint gravity_, CollisionType colliderType_, SDL_Rect rect_, float mass_) :Body(BodyType::DYNAMIC_BODY, colliderType_, position_, rect_, mass_)
	{
		velocity = velocity_;
		gravityAcceleration = gravity_;
	}
	DynamicBody(fPoint position_, fPoint velocity_, fPoint gravity_, CollisionType colliderType_, CircleCollider circle_, float mass_) :Body(BodyType::DYNAMIC_BODY, colliderType_, position_, circle_, mass_)
	{
		velocity = velocity_;
		gravityAcceleration = gravity_;
	}

	// Set an specific gravity for this body. Not compatible with SetGlobalGravity() being active. Desable SetGlobalgravity() by inputting SetGlobalGravity(0.0f, 0.0f); or deleting the function
	void SetGravityAcceleration(fPoint& gravity);
	// Set an constant velocity for this body.
	void SetConstantVelocity(fPoint& constVelocity);
	// Allow body to move given an specific force. It needs an input conditional ("if (LEFT ARROW REPEATED) Move(...)"). It allows to limit the velocity. If {0.0f, 0.0f} is inputted, velocity limit will not occur
	void Move(float newtons = 5.0f, Direction dir = Direction::LEFT, float velocityLimiter = 0.0f);
	// Allow body to dash given an specific force. It needs an input conditional ("if (LEFT ARROW DOWN) Dash(...)"). It only works on Air. "dashLimit" argument sets how many times can a body dash on air. If limit = 0, dash limit will not occur
	void Dash(float newtons = 400.0f, DashDirection dDir = DashDirection::LEFT, uint dashLimit = 0);
	// Allow body to jump given an specific force. It needs an input conditional ("if (SPACE BAR PRESSED) Jump(...)"). Input true/false for double jump ability
	void Jump(float newtonsY = 300.0f, bool doubleJump = false);
	// Allow body to jump from a wall given an specific force. It needs an input conditional ("if (SPACE BAR PRESSED) WallJump(...)"). Input true/false for wall friction descendence
	void WallJump(fPoint newtons = {200.0f, 200.0f}, bool wallFriction = false);
	// Applies an specific force to this body
	void ApplyForce(fPoint newtons);
	// Applies an specific force to this body
	void ApplyForce(float newtonsX = 0, float newtonsY = 0);
	//This function returns a debug bool by inputting which one you want to receive
	bool GetDebugBools(DebugBools dBool) const
	{
		bool boolSelec = false;

		switch (dBool)
		{
		case DebugBools::GROUND: boolSelec = onGround; break;
		case DebugBools::ROOF: boolSelec = onRoof; break;
		case DebugBools::LEFT_WALL: boolSelec = onLeftWall; break;
		case DebugBools::RIGHT_WALL: boolSelec = onRightWall; break;
		case DebugBools::JUMP: boolSelec = onJump; break;
		case DebugBools::DOUBLE_JUMP: boolSelec = onDoubleJump; break;
		case DebugBools::WALL_JUMP: boolSelec = onWallJump; break;
		case DebugBools::DASH: boolSelec = onDash; break;
		}

		return boolSelec;
	}
	// Restart the accelerations/velocities applied to the body
	void ResetForces();

private:
	friend class Physics;
	void SecondNewton();
	void ApplyFriction(float dt);
	void ApplyBuoyancy();
	void ApplyHidroDrag();
	void ResetBools();

private:
	friend class Body;
	friend class Physics;
	fPoint velocity;
	fPoint gravityAcceleration;
	fPoint acceleration = {};
	fPoint sumForces = { 0.0f, 0.0f };
	fPoint coeficientRestitution = { 1.0f,1.0f };
	fPoint frictionCoeficient = { 0.0f,0.0f };
	float hydroControlParameter = 0.0f;
	DynArray<fPoint> forces = {};
	bool onGround = false, onRoof = false, onLeftWall = false, onRightWall = false, onJump = false, onDoubleJump = false, onWallJump = false, onDash = false, onAir = false, onWater = false;
	int dashCount = 0;
	//Will be deleted
	bool liftAeroActive = false;
	bool buoyancyActive = false;
};

// ------------------------------------------------------------------------------------------------------------------------------

class Physics
{
public:

	Physics();

	virtual ~Physics();

	bool Awake();

	bool Start();

	//Update and step your physics
	bool Update(float dt);

	//Draw your physic bodies. First argument shows specific propierties from the given DynamicBody*
	void Draw(DynamicBody* dB = nullptr);

	bool CleanUp();

public:
	//Creates a new body with a rectangular form
	Body* CreateBody(BodyType bodyType_, fPoint position_ = { 0.0f,0.0f }, SDL_Rect rect_ = {}, fPoint velocity_ = { 0.0f,0.0f }, fPoint gravity_ = { 0.0f,0.0f }, float mass_ = 1);
	//Creates a new body with a circular form
	Body* CreateBody(BodyType bodyType_, fPoint position_ = { 0.0f,0.0f }, CircleCollider collider_ = {}, fPoint velocity_ = { 0.0f,0.0f }, fPoint gravity_ = { 0.0f,0.0f }, float mass_ = 1);
	//Sets global gravity for all bodies. {0.0f, 0.0f} for no gravity
	void SetGlobalGravity(fPoint gravity);
	//Sets global restitution coeficient for all bodies. {0.0f, 0.0f} for perfect elastic restitution
	void SetGlobalRestitution(fPoint restCoeficient);
	//Sets global friction for all bodies. {0.0f, 0.0f} for no friction
	void SetGlobalFriction(fPoint frictionCoeficient);
	//Sets a default physics presset
	void SetPhysicsPreset(PhysicsPreset phPreset);
	//Sets a default scenario presset
	void SetScenarioPreset(ScenarioPreset sPreset);
	//Kills a body when outside the rect inputted
	void DeathLimit(SDL_Rect limits);
	//Kills all static bodies in the screen
	void DestroyScenario();
	//Pauses all physics. Depending of true/false inputted, will pause/unpause the physics respectively
	void PausePhysics(bool pause);

public:
	bool debug = true;
	bool debugBools = false;

private:
	// Debug
	void Step(float dt);
	void Integrate(DynamicBody* item, float dt);
	void CheckCollisions();
	void ChangeGravityAcceleration(fPoint acceleration);
	void ChangeRestitutionCoeficient(fPoint restCoeficient);
	void ChangeFrictionCoeficient(fPoint frictCoeficient);
	void DebugDraw();
	void DebugBools(DynamicBody* dB = nullptr);
	void ResetBodyBools();

private:
	List<Body*> bodyList;
	Collision collisionUtil;
	fPoint globalGravity = {};
	fPoint globalRestitution = {};
	fPoint globalFriction = {};
	bool globalGravityActive = false;
	bool globalRestitutionActive = false;
	bool globalFrictionActive = false;
	bool globalPause = false;
};

#endif