#pragma once
#include "MathGeoLib/include/Math/Quat.h"
#include "Point3.h"

class Primitive
{
public:

	Primitive(const fPoint3& position, Quat& rotation, const fPoint3& normals) : Position(position), Rotation(rotation), Normals(normals)
	{
	}

	virtual ~Primitive()
	{
	}

	virtual void Draw() = 0;

public:
	fPoint3 Position;
	Quat Rotation;
	fPoint3 Normals;
};

