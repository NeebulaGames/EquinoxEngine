#ifndef __COORDINATE_ARROWS_H__
#define __COORDINATE_ARROWS_H__

#include "Primitive.h"

class CoordinateArrows :
	public Primitive
{
public:
	CoordinateArrows();

	void Draw() override;

private:
	float Origin[3] = {0, 0, 0};
	float XP[3] = {1, 0, 0}, YP[3] = {0, 1, 0}, ZP[3] = {0, 0, 1};
};

#endif
