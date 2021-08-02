#define _CRT_SECURE_NO_WARNINGS

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Physics.h"

#include "GuiString.h"
#include "GuiManager.h"

#include "Defs.h"
#include "Log.h"

Physics::Physics()
{

}

Physics::~Physics()
{}

bool Physics::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

bool Physics::Start()
{
	return true;
}

bool Physics::Update(float dt)
{
	if (!globalPause)
	{
		if (globalGravityActive) ChangeGravityAcceleration(globalGravity);

		if (globalRestitutionActive) ChangeRestitutionCoeficient(globalRestitution);
		else ChangeRestitutionCoeficient({ 1.0f, 1.0f });

		if (globalFrictionActive) ChangeFrictionCoeficient(globalFriction);

		Step(dt);

		if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
			debug = !debug;

		if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
			debugBools = !debugBools;
	}

	return true;
}

void Physics::Draw(DynamicBody* dB)
{
	if (debug) DebugDraw();
	if (debugBools) DebugBools(dB);
	ResetBodyBools();
}

bool Physics::CleanUp()
{
	LOG("Freeing scene");
	ListItem<Body*>* list;
	for (list = bodyList.start; list != NULL; list = list->next)
	{
		app->tex->UnLoad(list->data->texture);
		list->data->texture = nullptr;
		bodyList.Del(list);
		delete list->data;
	}
	bodyList.Clear();
	return true;
}

void Physics::DebugDraw()
{
	ListItem<Body*>* list;
	for (list = bodyList.start; list != NULL; list = list->next)
	{
		SDL_Rect rectx = {};
		SDL_Rect rect = {};
		CircleCollider circx;
		CircleCollider circ;

		switch (list->data->colliderType)
		{
		case CollisionType::RECTANGLE:
			rectx = { list->data->rect };
			rect = { METERS_TO_PIXELS(rectx.x), METERS_TO_PIXELS(rectx.y), METERS_TO_PIXELS(rectx.w), METERS_TO_PIXELS(rectx.h) };

			switch (list->data->bodyType)
			{
			case BodyType::DYNAMIC_BODY:
				app->render->DrawRectangle(rect, { 255, 200, 0, 150 });
				break;
			case BodyType::STATIC_BODY:
				app->render->DrawRectangle(rect, { 255, 0, 0, 150 });
				break;
			}
			break;

		case CollisionType::CIRCLE:
			circx = list->data->circle;
			circ = {(float)METERS_TO_PIXELS(circx.x), (float)METERS_TO_PIXELS(circx.y), (float)METERS_TO_PIXELS(circx.radius)};

			switch (list->data->bodyType)
			{
			case BodyType::DYNAMIC_BODY:
				app->render->DrawCircle(circ, { 255, 200, 0, 150 });
				break;
			case BodyType::STATIC_BODY:
				app->render->DrawCircle(circ, { 255, 0, 0, 150 });
				break;
			}
			break;
		}
	}
}

void Physics::DebugBools(DynamicBody* dB)
{
	bool dynamicProps = (dB != nullptr);
	bool ground = false;
	bool roof = false;
	bool jump = false;
	bool doubleJump = false;
	bool lWall = false;
	bool rWall = false;
	bool wallJump = false;
	bool dash = false;
	bool air = false;
	bool water = false;

	if (dynamicProps) app->render->DrawRectangle({ 0, 0, 360, 135 }, {180, 180, 180, 200});
	else app->render->DrawRectangle({ 0, 0, 180, 135 }, { 180, 180, 180, 200 });

	GuiString* n = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
	n->SetTextFont(app->fontTTF->defaultFont);

	n->bounds = { 5, 10, 25, 15 };
	n->SetString("GND:", BLACK);
	n->Draw();
	n->bounds.y = 35;
	n->SetString("LWL:", BLACK);
	n->Draw();
	n->bounds.y = 60;
	n->SetString("RWL:", BLACK);
	n->Draw();
	n->bounds.y = 85;
	n->SetString(" RF:", BLACK);
	n->Draw();
	n->bounds.y = 110;
	n->SetString("AIR:", BLACK);
	n->Draw();
	n->bounds = { 85, 10, 25, 15 };
	n->SetString("JMP:", BLACK);
	n->Draw();
	n->bounds.y = 35;
	n->SetString("DJP:", BLACK);
	n->Draw();
	n->bounds.y = 60;
	n->SetString("WJP:", BLACK);
	n->Draw();
	n->bounds.y = 85;
	n->SetString("DSH:", BLACK);
	n->Draw();
	n->bounds.y = 110;
	n->SetString("WTR:", BLACK);
	n->Draw();
	if (dynamicProps)
	{
		n->bounds = { 165, 10, 25, 15 };
		char posStr[40] = {};
		sprintf(posStr, "POS: %.2f %.2f", dB->position.x, dB->position.y);
		n->SetString(posStr, BLACK);
		n->Draw();
		n->bounds.y = 35;
		char velStr[40] = {};
		sprintf(velStr, "VEL: %.2f %.2f", dB->velocity.x, dB->velocity.y);
		n->SetString(velStr, BLACK);
		n->Draw();
		n->bounds.y = 60;
		char accStr[40] = {};
		sprintf(accStr, "ACC: %.2f %.2f", ((dB->velocity.x / app->GetDeltaTime()) * dB->mass), ((dB->velocity.y / app->GetDeltaTime()) * dB->mass));
		n->SetString(accStr, BLACK);
		n->Draw();
	}

	n->Delete();
	app->guiManager->DestroyGuiControl(n);
	n = nullptr;

	int xOff = 45;
	int xOff1 = 125; // xOff + 60
	int yOff = 2;
	ListItem<Body*>* list;
	for (list = bodyList.start; list != NULL; list = list->next)
	{
		if (list->data->bodyType == DYNAMIC_BODY)
		{
			DynamicBody* dB = (DynamicBody*)list->data;
			if (dB->onGround && !ground)
			{
				app->render->DrawRectangle({ xOff, 10 - yOff, 25, 15 }, { 255, 0, 0, 255 });
				ground = true;
			}
			
			if (dB->onLeftWall && !lWall)
			{
				app->render->DrawRectangle({ xOff, 35 - yOff, 25, 15 }, { 0, 255, 0, 255 });
				lWall = true;
			}

			if (dB->onRightWall && !rWall)
			{
				app->render->DrawRectangle({ xOff, 60 - yOff, 25, 15 }, { 255, 255, 0, 255 });
				rWall = true;
			}
			
			if (dB->onRoof && !roof)
			{
				app->render->DrawRectangle({ xOff, 85 - yOff, 25, 15 }, { 0, 0, 255, 255 });
				roof = true;
			}

			if (dB->onAir && !air)
			{
				app->render->DrawRectangle({ xOff, 110 - yOff, 25, 15 }, { 225, 105, 55, 255 });
				air = true;
			}

			if (dB->onJump && !jump)
			{
				app->render->DrawRectangle({ xOff1, 10 - yOff, 25, 15 }, { 0, 255, 255, 255 });
				jump = true;
			}

			if (dB->onDoubleJump && !doubleJump)
			{
				app->render->DrawRectangle({ xOff1, 35 - yOff, 25, 15 }, { 255, 0, 255, 255 });
				doubleJump = true;
			}

			if (dB->onWallJump && !wallJump)
			{
				app->render->DrawRectangle({ xOff1, 60 - yOff, 25, 15 }, { 255, 255, 255, 255 });
				wallJump = true;
			}

			if (dB->onDash && !dash)
			{
				app->render->DrawRectangle({ xOff1, 85 - yOff, 25, 15 }, { 225, 55, 105, 255 });
				dash = true;
			}

			if (dB->onWater && !water)
			{
				app->render->DrawRectangle({ xOff1, 110 - yOff, 25, 15 }, { 55, 225, 105, 255 });
				water = true;
			}
		}
	}
}

void Physics::ResetBodyBools()
{
	ListItem<Body*>* list;
	for (list = bodyList.start; list != NULL; list = list->next)
	{
		if (list->data->bodyType == DYNAMIC_BODY)
		{
			DynamicBody* dB = (DynamicBody*)list->data;
			dB->ResetBools();
		}
	}
}

void Physics::Integrate(DynamicBody* item, float dt)
{
	// Sum of all acceleration in both axis
	// sum of all forces
	// second law newton (divide by mass) and gett total acceleration

	item->position.x += item->velocity.x * dt + 0.5f * item->acceleration.x * dt * dt;
	item->velocity.x += item->acceleration.x * dt;

	item->position.y += item->velocity.y * dt + 0.5f * item->acceleration.y * dt * dt;
	item->velocity.y += item->acceleration.y * dt;
}

void Physics::Step(float dt)
{
	ListItem<Body*>* list;
	for (list = bodyList.start; list != NULL; list = list->next)
	{
		switch (list->data->bodyType)
		{
		case BodyType::DYNAMIC_BODY:
			DynamicBody* dB = (DynamicBody*)list->data;
			// Correct angles rotation
			dB->rotation = dB->ToPositiveAngle(dB->rotation);

			//dynamicBody->ApplyHidroDrag();
			//dynamicBody->ApplyBuoyancy();
			if (globalFrictionActive) dB->ApplyFriction(dt);
			//dynamicBody->ApplyAeroLift();

			// Second law newton
			dB->SecondNewton();

			// Applying gravity acceleration post second law newton
			dB->acceleration += dB->gravityAcceleration;

			// Integrate
			Integrate(dB, dt);

			dB->acceleration = { 0.0f,0.0f };

			// Setting rect collider position
			switch (dB->colliderType)
			{
			case CollisionType::RECTANGLE:
				dB->rect.x = (int)dB->position.x;
				dB->rect.y = (int)dB->position.y;
				break;

			case CollisionType::CIRCLE:
				dB->circle.x = (int)dB->position.x;
				dB->circle.y = (int)dB->position.y;
				break;
			}

			// Check Colls
			CheckCollisions();

			// onAir check
			if (!dB->onGround && !dB->onLeftWall && !dB->onRightWall && !dB->onRoof && !dB->onJump && !dB->onDoubleJump && !dB->onDash && !dB->onWallJump && !dB->onWater) dB->onAir = true;
			else dB->onAir = false;
			break;
		}
	}
}

void Physics::SetGlobalGravity(fPoint gravity)
{
	if (gravity == fPoint{ 0.0f, 0.0f }) globalGravityActive = false;
	else
	{
		globalGravityActive = true;
		globalGravity = gravity;
	}
}

void Physics::ChangeGravityAcceleration(fPoint acceleration)
{
	ListItem<Body*>* list;
	for (list = bodyList.start; list != NULL; list = list->next)
	{
		switch (list->data->bodyType)
		{
		case BodyType::DYNAMIC_BODY:
			DynamicBody* dB = (DynamicBody*)list->data;
			acceleration *= REALITY_MULTIPLIER;
			dB->gravityAcceleration = acceleration;
			break;
		}
	}
}

void Physics::SetGlobalRestitution(fPoint restCoeficient)
{
	if (restCoeficient == fPoint{ 0.0f, 0.0f }) globalRestitutionActive = false;
	else
	{
		globalRestitutionActive = true;
		globalRestitution = restCoeficient;
	}
}

void Physics::ChangeRestitutionCoeficient(fPoint restCoeficient)
{
	ListItem<Body*>* list;
	for (list = bodyList.start; list != NULL; list = list->next)
	{
		switch (list->data->bodyType)
		{
		case BodyType::DYNAMIC_BODY:
			DynamicBody* dB = (DynamicBody*)list->data;
			dB->coeficientRestitution = restCoeficient;
			break;
		}
	}
}

void Physics::SetGlobalFriction(fPoint frictionCoeficient)
{
	if (frictionCoeficient == fPoint{ 0.0f, 0.0f }) globalFrictionActive = false;
	else
	{
		globalFrictionActive = true;
		globalFriction = frictionCoeficient;
	}
}

void Physics::ChangeFrictionCoeficient(fPoint frictCoeficient)
{
	ListItem<Body*>* list;
	for (list = bodyList.start; list != NULL; list = list->next)
	{
		switch (list->data->bodyType)
		{
		case BodyType::DYNAMIC_BODY:
			DynamicBody* dB = (DynamicBody*)list->data;
			dB->frictionCoeficient = frictCoeficient;
			break;
		}
	}
}

void Physics::SetPhysicsPreset(PhysicsPreset phPreset)
{
	switch (phPreset)
	{
	case PhysicsPreset::PLATFORMER_PHYSICS_PRESET:
		SetGlobalGravity(fPoint{ 0.0f, 10.0f });
		SetGlobalRestitution(fPoint{ 0.02f, 0.1f });
		SetGlobalFriction(fPoint{0.1f, 0.0f});
		break;

	case PhysicsPreset::MOON_PHYSICS_PRESET:
		SetGlobalGravity(fPoint{ 0.0f, 1.62f });
		SetGlobalRestitution(fPoint{ 1.0f, 1.0f });
		SetGlobalFriction(fPoint{ 0.02f, 0.0f });
		break;

	case PhysicsPreset::SPACE_PHYSICS_PRESET:
		SetGlobalGravity(fPoint{ 0.0f, 0.0f });
		SetGlobalRestitution(fPoint{ 1.0f, 1.0f });
		SetGlobalFriction(fPoint{ 0.0f, 0.0f });
		break;
	}
}

void Physics::SetScenarioPreset(ScenarioPreset sPreset)
{
	DestroyScenario();

	switch (sPreset)
	{
	case ScenarioPreset::LIMITS_1280x720_SCENARIO_PRESET:
		//Limits
		CreateBody(BodyType::STATIC_BODY, fPoint{ 0, 0 }, { 0, 0, 1280, 50 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, fPoint{ 0, 0 }, { 0, 0, 50, 720 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, fPoint{ 0, 670 }, { 0, 670, 1280, 50 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, fPoint{ 1230, 0 }, { 1230, 0, 50, 720 }, { 0, 0 }, { 0, 0 }, 1);
		debugBools = false;
		break;

	case ScenarioPreset::PLATFORMER_1280x720_SCENARIO_PRESET:
		//Limits
		CreateBody(BodyType::STATIC_BODY, fPoint{ 0, 0 }, { 0, 0, 1280, 50 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, fPoint{ 0, 0 }, { 0, 0, 50, 720 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, fPoint{ 0, 670 }, { 0, 670, 1280, 50 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, fPoint{ 1230, 0 }, { 1230, 0, 50, 720 }, { 0, 0 }, { 0, 0 }, 1);
		//Obstacles
		CreateBody(BodyType::STATIC_BODY, fPoint{ 340, 490 }, { 340, 490, 600, 25 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, fPoint{ 540, 220 }, { 540, 220, 200, 25 }, { 0, 0 }, { 0, 0 }, 1);
		debugBools = false;
		break;

	case ScenarioPreset::WALLJUMP_1280x720_SCENARIO_PRESET:
		//Limits
		CreateBody(BodyType::STATIC_BODY, fPoint{ 0, 0 }, { 0, 0, 1280, 50 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, fPoint{ 0, 0 }, { 0, 0, 50, 720 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, fPoint{ 0, 670 }, { 0, 670, 1280, 50 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, fPoint{ 1230, 0 }, { 1230, 0, 50, 720 }, { 0, 0 }, { 0, 0 }, 1);
		//Obstacles
		CreateBody(BodyType::STATIC_BODY, fPoint{ 200, 200 }, { 200, 200, 25, 400 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, fPoint{ 500, 200 }, { 500, 200, 25, 400 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, fPoint{ 780, 200 }, { 780, 200, 25, 400 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, fPoint{ 1080, 200 }, { 1080, 200, 25, 400 }, { 0, 0 }, { 0, 0 }, 1);
		debugBools = false;
		break;

	case ScenarioPreset::SPHERE_1280x720_SCENARIO_PRESET:
		//Limits
		CreateBody(BodyType::STATIC_BODY, fPoint{ 0, 0 }, { 0, 0, 1280, 50 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, fPoint{ 0, 0 }, { 0, 0, 50, 720 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, fPoint{ 0, 670 }, { 0, 670, 1280, 50 }, { 0, 0 }, { 0, 0 }, 1);
		CreateBody(BodyType::STATIC_BODY, fPoint{ 1230, 0 }, { 1230, 0, 50, 720 }, { 0, 0 }, { 0, 0 }, 1);
		//Sphere
		CreateBody(BodyType::STATIC_BODY, fPoint{ 640, 360 }, CircleCollider{ 640, 390, 120}, { 0, 0 }, { 0, 0 }, 1);
		debugBools = false;
		break;
	}
}

void Physics::DeathLimit(SDL_Rect limits)
{
	ListItem<Body*>* list;

	for (list = bodyList.start; list != NULL; list = list->next)
	{
		switch (list->data->colliderType)
		{
		case CollisionType::RECTANGLE:
			if (!collisionUtil.CheckCollision(list->data->rect, limits))
			{
				app->tex->UnLoad(list->data->texture);
				list->data->texture = nullptr;
				bodyList.Del(list);
				delete list->data;
			}
			break;

		case CollisionType::CIRCLE:
			if (!collisionUtil.CheckCollision(list->data->circle, limits))
			{
				app->tex->UnLoad(list->data->texture);
				list->data->texture = nullptr;
				bodyList.Del(list);
				delete list->data;
			}
			break;
		}
	}
}

void Physics::DestroyScenario()
{
	ListItem<Body*>* list;
	for (list = bodyList.start; list != NULL; list = list->next)
	{
		if (list->data->bodyType == BodyType::STATIC_BODY)
		{
			app->tex->UnLoad(list->data->texture);
			list->data->texture = nullptr;
			bodyList.Del(list);
			delete list->data;
		}
	}
	bodyList.Clear();
	return;
}

void Physics::PausePhysics(bool pause)
{
	globalPause = pause;
}

void Physics::CheckCollisions()
{
	ListItem<Body*>* bodyList1;
	ListItem<Body*>* bodyList2;
	for (bodyList1 = bodyList.start; bodyList1 != NULL; bodyList1 = bodyList1->next)
	{
		for (bodyList2 = bodyList.start; bodyList2 != NULL; bodyList2 = bodyList2->next)
		{
			if (bodyList1->data != bodyList2->data)
			{
				bool collided = false;
				if (bodyList1->data->colliderType == RECTANGLE && bodyList2->data->colliderType == RECTANGLE)
				{
					if (collisionUtil.CheckCollision(bodyList1->data->rect, bodyList2->data->rect))
					{
						collided = true;
					}
				}
				else if (bodyList1->data->colliderType == CIRCLE && bodyList2->data->colliderType == RECTANGLE)
				{
					if (collisionUtil.CheckCollision(bodyList1->data->circle, bodyList2->data->rect))
					{
						collided = true;
					}
				}
				else if (bodyList1->data->colliderType == RECTANGLE && bodyList2->data->colliderType == CIRCLE)
				{
					if (collisionUtil.CheckCollision(bodyList1->data->rect, bodyList2->data->circle)) collided = true;
				}
				else if (bodyList1->data->colliderType == CIRCLE && bodyList2->data->colliderType == CIRCLE)
				{
					if (collisionUtil.CheckCollision(bodyList1->data->circle, bodyList2->data->circle)) collided = true;
				}

				if (collided)
				{
					bodyList1->data->SolveCollision(*bodyList2->data);
					bodyList2->data->SolveCollision(*bodyList1->data);
				}
			}
		}
	}
}

Body* Physics::CreateBody(BodyType bodyType_, fPoint position_ , SDL_Rect rect_, fPoint velocity_, fPoint gravity_, float mass_)
{
	DynamicBody* newBodyD = nullptr;
	StaticBody* newBodyS = nullptr;
	switch (bodyType_)
	{
	case BodyType::DYNAMIC_BODY:
		newBodyD = new DynamicBody(position_, velocity_, gravity_, RECTANGLE, rect_, mass_);
		bodyList.Add(newBodyD);
		newBodyD = nullptr;
		break;

	case BodyType::STATIC_BODY:
		newBodyS = new StaticBody(position_, RECTANGLE, rect_, mass_);
		bodyList.Add(newBodyS);
		newBodyS = nullptr;
		break;
	}

	return bodyList.end->data;
}

Body* Physics::CreateBody(BodyType bodyType_, fPoint position_, CircleCollider circle_, fPoint velocity_, fPoint gravity_, float mass_)
{
	DynamicBody* newBodyD = nullptr;
	StaticBody* newBodyS = nullptr;

	switch (bodyType_)
	{
	case BodyType::DYNAMIC_BODY:
		newBodyD = new DynamicBody(position_, velocity_, gravity_, CIRCLE, circle_, mass_);
		bodyList.Add(newBodyD);
		newBodyD = nullptr;
		break;

	case BodyType::STATIC_BODY:
		newBodyS = new StaticBody(position_, CIRCLE, circle_, mass_);
		bodyList.Add(newBodyS);
		newBodyS = nullptr;
		break;
	}

	return bodyList.end->data;
}

void DynamicBody::SetGravityAcceleration(fPoint& gravity)
{
	this->gravityAcceleration = gravity;
}

void DynamicBody::SetConstantVelocity(fPoint& constVelocity)
{
	this->velocity = velocity;
}

void DynamicBody::Move(float newtons, Direction dir, float velocityLimiter)
{
	newtons = abs(newtons);
	velocityLimiter = abs(velocityLimiter);
	bool vL = (velocityLimiter == 0.0f);

	switch (dir)
	{
	case Direction::UP:
		if (vL) this->ApplyForce({ 0.0f, -newtons });
		else
		{
			if (velocity.y >= -velocityLimiter) this->ApplyForce({ 0.0f, -newtons });
		}
		break;

	case Direction::DOWN:
		if (vL) this->ApplyForce({ 0.0f, newtons });
		else
		{
			if (velocity.y <= velocityLimiter) this->ApplyForce({ 0.0f, newtons });
		}
		break;

	case Direction::LEFT:
		if (vL) this->ApplyForce({ -newtons, 0.0f });
		else
		{
			if (velocity.x >= -velocityLimiter) this->ApplyForce({ -newtons, 0.0f });
		}
		break;

	case Direction::RIGHT:
		if (vL) this->ApplyForce({ newtons, 0.0f });
		else
		{
			if (velocity.x <= velocityLimiter) this->ApplyForce({ newtons, 0.0f });
		}
		break;
	}
}

void DynamicBody::Dash(float newtons, DashDirection dDir, uint dashLimit)
{
	if (!onGround && !onLeftWall && !onRightWall && !onRoof) //if (onAir)
	{
		if (dashLimit == 0 || dashLimit > dashCount)
		{
			if (dashLimit != 0) dashCount++;

			this->onDash = true;
			this->onJump = false;
			this->onDoubleJump = false;
			this->onWallJump = false;

			newtons = abs(newtons);

			switch (dDir)
			{
			case DashDirection::UP:
				this->ApplyForce({ 0.0f, -newtons });
				break;

			case DashDirection::DOWN:
				this->ApplyForce({ 0.0f, newtons });
				break;

			case DashDirection::LEFT:
				this->ApplyForce({ -newtons, 0.0f });
				break;

			case DashDirection::RIGHT:
				this->ApplyForce({ newtons, 0.0f });
				break;

			case DashDirection::UPLEFT:
				this->ApplyForce({ -newtons, -newtons });
				break;

			case DashDirection::DOWNLEFT:
				this->ApplyForce({ -newtons, newtons });
				break;

			case DashDirection::UPRIGHT:
				this->ApplyForce({ newtons, -newtons });
				break;

			case DashDirection::DOWNRIGHT:
				this->ApplyForce({ newtons, newtons });
				break;
			}
		}
	}
}

void DynamicBody::Jump(float newtonsY, bool doubleJump)
{
	newtonsY = abs(newtonsY) * -1;

	if (doubleJump && !onDoubleJump && !onWallJump && onJump && !onLeftWall && !onRightWall)
	{
		this->ApplyForce({ 0.0f, newtonsY / 2 });
		onDoubleJump = true;
		onJump = false;
	}

	if (onGround && !onJump)
	{
		this->ApplyForce({0.0f, newtonsY });
		onJump = true;
	}
}

void DynamicBody::WallJump(fPoint newtons, bool wallFriction)
{
	newtons.Absolute();
	if (onLeftWall && !onGround && !onWallJump)
	{
		this->ApplyForce({ newtons.x, -newtons.y });
		onWallJump = true;
		onJump = false;
		onDoubleJump = false;
		onDash = false;
	}

	if (onRightWall && !onGround && !onWallJump)
	{
		this->ApplyForce({ -newtons.x, -newtons.y });
		onWallJump = true;
		onJump = false;
		onDoubleJump = false;
	}
}

void DynamicBody::ApplyForce(fPoint newtons)
{
	newtons *= NEWTONS_MULTIPLIER;
	forces.PushBack(newtons);
}

void DynamicBody::ApplyForce(float newtonsX, float newtonsY)
{
	fPoint newtons = {newtonsX, newtonsY};
	newtons *= NEWTONS_MULTIPLIER;
	forces.PushBack(newtons);
}

void DynamicBody::ResetForces()
{
	this->acceleration.SetToZero();
	this->velocity.SetToZero();
	this->sumForces.SetToZero();
	this->forces.Clear();
}

void DynamicBody::SecondNewton()
{
	for (int i = 0; i < forces.Count(); i++)
	{
		sumForces += *forces.At(i);
		forces.Pop(*forces.At(i));
	}
	forces.Clear();

	acceleration.x += sumForces.x / mass;
	acceleration.y += sumForces.y / mass;

	sumForces = { 0.0f,0.0f };
}

void DynamicBody::ApplyFriction(float dt)
{
	//Soc molt bo. Estic calculant la força que necessito per parar el cos. La redueixo i la converteixo en friction

	fPoint dragForce = {0.0f, 0.0f};
	dragForce.x = (velocity.x / dt) * mass * frictionCoeficient.x;
	dragForce.y = (velocity.y / dt) * mass * frictionCoeficient.y;

	dragForce.Negate();

	forces.PushBack(dragForce);
}

void DynamicBody::ApplyBuoyancy()
{
	if (buoyancyActive)
	{
		fPoint buoyancyForce = this->gravityAcceleration;

		float magnitude = sqrt(pow(this->gravityAcceleration.x, 2) + pow(this->gravityAcceleration.y, 2));

		buoyancyForce = { buoyancyForce.x / magnitude, buoyancyForce.y / magnitude };
		buoyancyForce.Negate();

		fPoint buoyancyForceMagnitude = { 0,0 };
		buoyancyForceMagnitude.x = mass * this->gravityAcceleration.x * velocity.x - mass * this->gravityAcceleration.x;
		buoyancyForceMagnitude.y = mass * this->gravityAcceleration.y * velocity.y - mass * this->gravityAcceleration.y;

		buoyancyForce.x = buoyancyForce.x * buoyancyForceMagnitude.x;
		buoyancyForce.y = buoyancyForce.y * buoyancyForceMagnitude.y;

		forces.PushBack(buoyancyForce);
	}
}

void DynamicBody::ApplyHidroDrag()
{
	if (buoyancyActive)
	{
		fPoint hidroDrag = velocity;

		float magnitude = sqrt(pow(this->velocity.x, 2) + pow(this->velocity.y, 2));

		hidroDrag = { hidroDrag.x / magnitude, hidroDrag.y / magnitude };
		hidroDrag.Negate();

		fPoint hidroDragMagnitude = { 0,0 };
		hidroDragMagnitude.x =velocity.x * this->hydroControlParameter;
		hidroDragMagnitude.y =velocity.y * this->hydroControlParameter;

		hidroDrag.x = hidroDrag.x * hidroDragMagnitude.x;
		hidroDrag.y = hidroDrag.y * hidroDragMagnitude.y;

		forces.PushBack(hidroDrag);
	}
}

void DynamicBody::ResetBools()
{
	this->onGround = false;
	this->onRoof = false;
	this->onLeftWall = false;
	this->onRightWall = false;
}

void Body::SolveCollision(Body &body)
{
	this->DeClipper(body); // First declipp then do anything 
}

void Body::DeClipper(Body &body)
{
	if (body.isCollidable)
	{
		switch (this->bodyType)
		{
		case BodyType::DYNAMIC_BODY:
			DynamicBody* currentBody = (DynamicBody*)this;
			
			if (currentBody->colliderType == RECTANGLE && body.colliderType == RECTANGLE)
			{
				//TOP & DOWN
				if (currentBody->position.y + currentBody->rect.h >= body.position.y && !(currentBody->position.y >= body.position.y) && (currentBody->position.y + currentBody->rect.h <= body.position.y + body.rect.h))
				{
					// Ground
					currentBody->position.y = body.rect.y - currentBody->rect.h;
					currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;

					currentBody->onGround = true;
					currentBody->dashCount = 0;
					if (currentBody->onJump)
					{
						currentBody->onJump = false;
						if (currentBody->onDoubleJump) currentBody->onDoubleJump = false;
					}
					if (currentBody->onDoubleJump) currentBody->onDoubleJump = false;
					if (currentBody->onWallJump) currentBody->onWallJump = false;
					if (currentBody->onDash) currentBody->onDash = false;
				}
				else if ((currentBody->position.y <= body.position.y + body.rect.h) && currentBody->position.y >= body.position.y && !(currentBody->position.y + currentBody->rect.h <= body.position.y + body.rect.h))
				{
					// Top
					currentBody->position.y = body.rect.y + body.rect.h;
					currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;
					currentBody->onRoof = true;
				}

				//LEFT & RIGHT
				if ((currentBody->position.x <= body.position.x + body.rect.w) && currentBody->position.x >= body.position.x && !(currentBody->position.x + currentBody->rect.w <= body.position.x + body.rect.w))
				{
					// Left wall
					currentBody->position.x = body.rect.x + body.rect.w;
					currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;
					currentBody->onLeftWall = true;
					if (currentBody->onWallJump) currentBody->onWallJump = false;
				}
				else if ((currentBody->position.x + currentBody->rect.w >= body.position.x) && !(currentBody->position.x >= body.position.x))
				{
					// Right wall
					currentBody->position.x = body.rect.x - currentBody->rect.w;
					currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;
					currentBody->onRightWall = true;
					if (currentBody->onWallJump) currentBody->onWallJump = false;
				}

				//POSITION SET
				currentBody->rect.x = (int)currentBody->position.x;
				currentBody->rect.y = (int)currentBody->position.y;
			}
			else if (currentBody->colliderType == RECTANGLE && body.colliderType == CIRCLE)
			{
				//TOP & DOWN
				if ((currentBody->position.y + currentBody->rect.h >= body.circle.y - body.circle.radius) && !(currentBody->position.y >= body.circle.y - body.circle.radius) && (currentBody->position.y + currentBody->rect.h <= body.circle.y + body.circle.radius))
				{
					// Ground
					currentBody->position.y = body.circle.y - body.circle.radius - currentBody->rect.h;
					currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;

					currentBody->onGround = true;
					currentBody->dashCount = 0;
					if (currentBody->onJump)
					{
						currentBody->onJump = false;
						if (currentBody->onDoubleJump) currentBody->onDoubleJump = false;
					}
					if (currentBody->onDoubleJump) currentBody->onDoubleJump = false;
					if (currentBody->onWallJump) currentBody->onWallJump = false;
					if (currentBody->onDash) currentBody->onDash = false;
				}
				else if ((currentBody->position.y <= body.circle.y + body.circle.radius) && (currentBody->position.y >= body.circle.y - body.circle.radius) && !(currentBody->position.y + currentBody->rect.h <= body.position.y + body.circle.radius))
				{
					// Top
					currentBody->position.y = body.circle.y + body.circle.radius;
					currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;
					currentBody->onRoof = true;
				}

				//LEFT & RIGHT
				if ((currentBody->position.x <= body.circle.x + body.circle.radius) && (currentBody->position.x >= body.circle.x - body.circle.radius) && !(currentBody->position.x + currentBody->rect.w <= body.position.x + body.circle.radius))
				{
					// Left wall
					currentBody->position.x = body.circle.x + body.circle.radius;
					currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;

					currentBody->onLeftWall = true;
					if (currentBody->onWallJump) currentBody->onWallJump = false;
				}
				else if ((currentBody->position.x + currentBody->rect.w >= body.circle.x - body.circle.radius) && !(currentBody->position.x >= body.circle.x - body.circle.radius) && (currentBody->position.x + currentBody->rect.w <= body.circle.x + body.circle.radius))
				{
					// Right wall
					currentBody->position.x = body.circle.x - body.circle.radius - currentBody->rect.w;
					currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;
					currentBody->onRightWall = true;
					if (currentBody->onWallJump) currentBody->onWallJump = false;
				}

				//POSITION SET
				currentBody->circle.x = (int)currentBody->position.x;
				currentBody->circle.y = (int)currentBody->position.y;
			}
			else if (currentBody->colliderType == CIRCLE && body.colliderType == RECTANGLE)
			{
				//TOP & DOWN
				if (currentBody->position.y + currentBody->rect.h >= body.position.y && !(currentBody->position.y >= body.position.y) && (currentBody->position.y + currentBody->rect.h <= body.position.y + body.circle.GetDiameter()))
				{
					// Ground
					currentBody->position.y = body.circle.y - currentBody->rect.h;
					currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;

					currentBody->dashCount = 0;
					currentBody->onGround = true;
					if (currentBody->onJump)
					{
						currentBody->onJump = false;
						if (currentBody->onDoubleJump) currentBody->onDoubleJump = false;
					}
					if (currentBody->onDoubleJump) currentBody->onDoubleJump = false;
					if (currentBody->onWallJump) currentBody->onWallJump = false;
					if (currentBody->onDash) currentBody->onDash = false;
				}
				else if ((currentBody->position.y <= body.position.y + body.circle.GetDiameter()) && currentBody->position.y >= body.position.y && !(currentBody->position.y + currentBody->rect.h <= body.position.y + body.circle.GetDiameter()))
				{
					// Top
					currentBody->position.y = body.circle.y + body.circle.radius;
					currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;
					currentBody->onRoof = true;
				}

				//LEFT & RIGHT
				if ((currentBody->position.x <= body.position.x + body.circle.GetDiameter()) && currentBody->position.x >= body.position.x && !(currentBody->position.x + currentBody->rect.w <= body.position.x + body.circle.GetDiameter()))
				{
					// Left wall
					currentBody->position.x = body.circle.x + body.circle.radius;
					currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;
					currentBody->onLeftWall = true;
					if (currentBody->onWallJump) currentBody->onWallJump = false;
				}
				else if ((currentBody->position.x + currentBody->rect.w >= body.position.x) && !(currentBody->position.x >= body.position.x))
				{
					// Right wall
					currentBody->position.x = body.circle.x - currentBody->rect.w;
					currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;
					currentBody->onRightWall = true;
					if (currentBody->onWallJump) currentBody->onWallJump = false;
				}

				//POSITION SET
				currentBody->rect.x = (int)currentBody->position.x;
				currentBody->rect.y = (int)currentBody->position.y;
			}
			else if (currentBody->colliderType == CIRCLE && body.colliderType == CIRCLE)
			{
				//TOP & DOWN
				if (currentBody->position.y + currentBody->circle.GetDiameter() >= body.position.y && !(currentBody->position.y >= body.position.y) && (currentBody->position.y + currentBody->circle.GetDiameter() <= body.position.y + body.circle.GetDiameter()))
				{
					// Ground
					currentBody->position.y = body.circle.y - currentBody->circle.radius;
					currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;

					currentBody->onGround = true;
					currentBody->dashCount = 0;
					if (currentBody->onJump)
					{
						currentBody->onJump = false;
						if (currentBody->onDoubleJump) currentBody->onDoubleJump = false;
					}
					if (currentBody->onDoubleJump) currentBody->onDoubleJump = false;
					if (currentBody->onWallJump) currentBody->onWallJump = false;
					if (currentBody->onDash) currentBody->onDash = false;
				}
				else if ((currentBody->position.y <= body.position.y + body.circle.GetDiameter()) && currentBody->position.y >= body.position.y && !(currentBody->position.y + currentBody->circle.GetDiameter() <= body.position.y + body.circle.GetDiameter()))
				{
					// Top
					currentBody->position.y = body.circle.y + body.circle.radius;
					currentBody->velocity.y = -currentBody->velocity.y * currentBody->coeficientRestitution.y;
					currentBody->onRoof = true;
				}

				//LEFT & RIGHT
				if ((currentBody->position.x <= body.position.x + body.circle.GetDiameter()) && currentBody->position.x >= body.position.x && !(currentBody->position.x + currentBody->circle.GetDiameter() <= body.position.x + body.circle.GetDiameter()))
				{
					// Left wall
					currentBody->position.x = body.circle.x + currentBody->circle.radius;
					currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;
					currentBody->onLeftWall = true;
					if (currentBody->onWallJump) currentBody->onWallJump = false;
				}
				else if ((currentBody->position.x + currentBody->circle.GetDiameter() >= body.position.x) && !(currentBody->position.x >= body.position.x))
				{
					// Right wall
					currentBody->position.x = body.circle.x - currentBody->circle.radius;
					currentBody->velocity.x = -currentBody->velocity.x * currentBody->coeficientRestitution.x;
					currentBody->onRightWall = true;
					if (currentBody->onWallJump) currentBody->onWallJump = false;
				}

				//POSITION SET
				currentBody->circle.x = (int)currentBody->position.x;
				currentBody->circle.y = (int)currentBody->position.y;
			}

			break;
		}
	}
}

double Body::ToPositiveAngle(double angle)
{
	angle = fmod(angle, 360);
	while (angle < 0) { //pretty sure this comparison is valid for doubles and floats
		angle += 360.0;
	}

	return angle;
}