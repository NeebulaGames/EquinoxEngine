#ifndef __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__
#include "BaseComponent.h"
#include <MathGeoLib/include/Math/float3.h>
#include <MathGeoLib/include/Math/Quat.h>

class TransformComponent :
	BaseComponent
{
public:
	float3 Position = float3(0, 0, 0);
	Quat Rotation = Quat(0, 0, 0, 1);
	float3 Scale = float3(1.f, 1.f, 1.f);

public:
	TransformComponent();
	~TransformComponent();

	void Update() override;
};

#endif
