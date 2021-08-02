#ifndef _SPLINE_H_
#define _SPLINE_H_

#include "Point.h"
#include "Log.h"
#include <vector>

#include "App.h"
#include "Render.h"
#include "Input.h"

struct sSpline
{
	std::vector<fPoint>	points;
	int	selectedPoint = 0;
	float splineLength = 0.0f;
	bool loop = false;

	fPoint GetSplinePoint(float t, bool looped = false)
	{
		int p0, p1, p2, p3;
		if (looped == false)
		{
			p1 = (int)t + 1;
			p2 = p1 + 1;
			p3 = p2 + 1;
			p0 = p1 - 1;
		}
		else
		{
			p1 = (int)t;
			p2 = (p1 + 1) % points.size();
			p3 = (p2 + 1) % points.size();
			p0 = p1 >= 1 ? p1 - 1 : points.size() - 1;
		}

		t = t - (int)t;

		float tt = t * t;
		float ttt = tt * t;

		float q1 = -ttt + 2.0f * tt - t;
		float q2 = 3.0f * ttt - 5.0f * tt + 2.0f;
		float q3 = -3.0f * ttt + 4.0f * tt + t;
		float q4 = ttt - tt;

		float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
		float ty = 0.5f * (points[p0].y * q1 + points[p1].y * q2 + points[p2].y * q3 + points[p3].y * q4);

		return{ tx, ty };
	}

	fPoint GetSplineGradient(float t, bool bLooped = false)
	{
		int p0, p1, p2, p3;
		if (!bLooped)
		{
			p1 = (int)t + 1;
			p2 = p1 + 1;
			p3 = p2 + 1;
			p0 = p1 - 1;
		}
		else
		{
			p1 = (int)t;
			p2 = (p1 + 1) % points.size();
			p3 = (p2 + 1) % points.size();
			p0 = p1 >= 1 ? p1 - 1 : points.size() - 1;
		}

		t = t - (int)t;

		float tt = t * t;
		float ttt = tt * t;

		float q1 = -3.0f * tt + 4.0f * t - 1;
		float q2 = 9.0f * tt - 10.0f * t;
		float q3 = -9.0f * tt + 8.0f * t + 1.0f;
		float q4 = 3.0f * tt - 2.0f * t;

		float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
		float ty = 0.5f * (points[p0].y * q1 + points[p1].y * q2 + points[p2].y * q3 + points[p3].y * q4);

		return{ tx, ty };
	}

	float CalculateSegmentLength(int node, bool bLooped = false)
	{
		float fLength = 0.0f;
		float fStepSize = 0.005;

		fPoint old_point, new_point;
		old_point = GetSplinePoint((float)node, bLooped);

		for (float t = 0; t < 1.0f; t += fStepSize)
		{
			new_point = GetSplinePoint((float)node + t, bLooped);
			fLength += sqrtf((new_point.x - old_point.x) * (new_point.x - old_point.x)
				+ (new_point.y - old_point.y) * (new_point.y - old_point.y));
			old_point = new_point;
		}

		return fLength;
	}

	float GetNormalisedOffset(float p)
	{
		// Which node is the base?
		int i = 0;
		while (p > points[i].length)
		{
			p -= points[i].length;
			i++;
		}

		// The fractional is the offset 
		return (float)i + (p / points[i].length);
	}
};

class Spline
{
public:
	Spline() {}
	~Spline() {}

	//DEBUG
	void DrawSpline(uint id);
	void DrawSplineControlPoints(uint id);

	void HandleInput(uint id);

	void Clear();
	void LoadSplines(pugi::xml_document&);

public:
	List<sSpline> splinesList;

private: //SPLINE CONSTRUCTION AND MODIFICATION

	void CreateSplines(pugi::xml_node& node);
	void SaveSplines();

	pugi::xml_node LoadSplineConfig(pugi::xml_document&);

	float fMarker = 0;

	SDL_Color black = { 0  , 0  , 0  , 255 };
	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Color red = { 255, 0  , 0  , 255 };
	SDL_Color green = { 0  , 255, 0  , 255 };
	SDL_Color blue  = { 0  , 0  , 255, 255 };
};
#endif //!_SPLINE_H_
