#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "List.h"
#include "SString.h"

#include "SDL/include/SDL_rect.h"

struct SDL_Rect;

struct CircleCollider
{
public:

	CircleCollider() {}
	CircleCollider(float x_, float y_, float radius_) : x(x_), y(y_), radius(radius_) {}
	float x = 0.0f, y = 0.0f;
	float radius = 0.0f;

	void SetPos(int x_, int y_)
	{
		x = x_;
		y = y_;
	}

	void SetRadius(int radius_)
	{
		radius = radius_;
	}

	void SetCircle(int x_, int y_, int radius_)
	{
		x = x_;
		y = y_;
		radius = radius_;
	}

	float GetDiameter() const
	{
		return (radius * 2.0f);
	}

	//fPoint GetAltLeftPosition()
	//{
	//	return {x - radius, y - radius};
	//}
};

#endif
