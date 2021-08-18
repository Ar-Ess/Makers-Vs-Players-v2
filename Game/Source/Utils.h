#ifndef __UTILS_H__
#define __UTILS_H__

#include "App.h"
#include "Render.h"

#include "List.h"
#include "SDL/include/SDL_rect.h"

struct SDL_Rect;

class Utils
{
public:
	Utils() {}

	void ApplyCameraPosition(int& x, int& y)
	{
		x -= app->render->camera.x;
		y -= app->render->camera.y;
	}

	void RemoveCameraPosition(int& x, int& y)
	{
		x += app->render->camera.x;
		y += app->render->camera.y;
	}

	double Distance(int x1, int y1, int x2, int y2)
	{
		return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	}

	bool CheckCollision(SDL_Rect r1, SDL_Rect r2) const
	{
		if ((r1.x > r2.x + r2.w) || (r1.x + r1.w < r2.x) || (r1.y > r2.y + r2.h) || (r1.y + r1.h < r2.y))
		{
			return false;
		}
		return true;
	}

	bool CheckCollision(CircleCollider& a, SDL_Rect& b)
	{
		//Closest point on collision box
		int cX, cY;

		//Find closest x offset
		if (a.x < b.x)
		{
			cX = b.x;
		}
		else if (a.x > b.x + b.w)
		{
			cX = b.x + b.w;
		}
		else
		{
			cX = a.x;
		}
		//Find closest y offset
		if (a.y < b.y)
		{
			cY = b.y;
		}
		else if (a.y > b.y + b.h)
		{
			cY = b.y + b.h;
		}
		else
		{
			cY = a.y;
		}

		//If the closest point is inside the circle
		if (int dist = Distance(a.x, a.y, cX, cY) < a.radius)
		{
			//This box and the circle have collided
			if (dist == 0.0f)
				return false;
			return true;
		}
		//If the shapes have not collided
		return false;
	}

	bool CheckCollision(SDL_Rect& b, CircleCollider& a)
	{
		//Closest point on collision box
		int cX, cY;

		//Find closest x offset
		if (a.x < b.x)
		{
			cX = b.x;
		}
		else if (a.x > b.x + b.w)
		{
			cX = b.x + b.w;
		}
		else
		{
			cX = a.x;
		}
		//Find closest y offset
		if (a.y < b.y)
		{
			cY = b.y;
		}
		else if (a.y > b.y + b.h)
		{
			cY = b.y + b.h;
		}
		else
		{
			cY = a.y;
		}

		//If the closest point is inside the circle
		double dist = Distance(a.x, a.y, cX, cY); // a.x a.y cX cY
		if (dist < a.radius)
		{
			//This box and the circle have collided
			if (dist == 0.0f)
				return false;
			return true;
		}
		//If the shapes have not collided
		return false;
	}

	bool CheckCollision(CircleCollider& A, CircleCollider& B)
	{
		//If the distance between the centers of the circles is less than the sum of their radii
		if (Distance(A.x, A.y, B.x, B.y) < (A.radius + B.radius))
		{
			//The circles have collided
			return true;
		}

		//If not
		return false;
	}

	SDL_Rect IntersectingRectangle(SDL_Rect r1, SDL_Rect r2)
	{
		int x = Max(r1.x, r2.x);
		int y = Min(r1.y, r2.y);
		int w = Min(r1.x + r1.w, r2.x + r2.w) - Max(r1.x, r2.x);
		int h = Min(r1.y + r1.h, r2.y + r2.h) - Max(r1.y, r2.y);

		return {x, y, w, h};
	}

	int Min(int a, int b)
	{
		if (a <= b) return a;
		return b;
	}
	float Min(float a, float b)
	{
		if (a <= b) return a;
		return b;
	}

	int Max(int a, int b)
	{
		if (a >= b) return a;
		return b;
	}
	float Max(float a, float b)
	{
		if (a >= b) return a;
		return b;
	}

};

#endif