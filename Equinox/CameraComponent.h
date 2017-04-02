#ifndef __COMPONENT_CAMERA_H__
#define __COMPONENT_CAMERA_H__

#include "BaseComponent.h"
#include <MathGeoLib/include/Geometry/Frustum.h>

class CameraComponent :
	public BaseComponent
{
public:
	CameraComponent();
	~CameraComponent();

	void Update(float dt) override;

	void SetFOV(float fov);
	void SetAspectRatio(float ratio);
	void SetPlaneDistances(float near, float far);
	float3 Position() const;
	float3 Orientation() const;
	void LookAt(float x, float y, float z);
	float* GetProjectionMatrix() const;
	float* GetViewMatrix() const;
	void SetFront(const vec &front);
	void SetUp(const vec &up);
	void SetPos(const vec &pos);
	const vec& GetUp() const;
	vec GetWorldRight() const;
	AABB GetFrustumAABB();

private:
	math::Frustum _frustum;

};

#endif