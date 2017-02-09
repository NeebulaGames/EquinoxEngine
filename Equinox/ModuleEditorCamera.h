#ifndef __MODULEEDITORCAMERA_H__
#define __MODULEEDITORCAMERA_H__

#include "Module.h"
#include <MathGeoLib/include/Geometry/Frustum.h>

class ModuleEditorCamera :
	public Module
{
public:
	ModuleEditorCamera();
	~ModuleEditorCamera();

	update_status Update() override;

	void SetFOV(float fov);
	void SetAspectRatio(float ratio);
	void SetPlaneDistances(float near, float far);
	float3 Position() const;
	float3 Orientation() const;
	void LookAt(float x, float y, float z);
	float* GetProjectionMatrix() const;
	float* GetViewMatrix() const;

public:
	math::Frustum Frustum;

};

#endif // __MODULEEDITORCAMERA_H__
