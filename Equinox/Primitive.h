#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "MathGeoLib/include/Math/Quat.h"
#include "Entity.h"

class Primitive
	: public Entity
{
public:

	/*
	 * Basic Primitive
	 */
	Primitive()
	{
	}

	/*
	 * Primitive with Position and Rotation
	 */
	Primitive(const float3& position, const Quat& rotation) : Position(position), Rotation(rotation)
	{
	}

	/*
	* Primitive with Position, Rotation and Color
	*/
	Primitive(const float3& position, const Quat& rotation, const float3& color) : Position(position), Rotation(rotation), Color(color)
	{
	}

	virtual ~Primitive()
	{
	}

	virtual void Draw() = 0;

public:
	float3 Position = float3(0, 0, 0);
	Quat Rotation = Quat::FromEulerXYZ(0, 0, 0);
	float3 Color = float3(0, 0, 0);
	float3 Scale = float3(1, 1, 1);
};

#endif // __PRIMITIVE_H__

