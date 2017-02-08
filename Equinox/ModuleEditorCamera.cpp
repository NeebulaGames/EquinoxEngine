#include "ModuleEditorCamera.h"
#include "Engine.h"
#include "ModuleInput.h"

using namespace math;

ModuleEditorCamera::ModuleEditorCamera()
{
	Frustum.SetPos(float3::zero);
	Frustum.SetFront(-float3::unitZ);
	Frustum.SetUp(float3::unitY);

	SetPlaneDistances(0.1f, 1000.0f);

	Frustum.SetPerspective(1, DegToRad(60));
	SetAspectRatio(1.3f);

	Frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
}

ModuleEditorCamera::~ModuleEditorCamera()
{
}

void ModuleEditorCamera::SetFOV(float fov)
{
	Frustum.SetVerticalFovAndAspectRatio(fov, Frustum.AspectRatio());
}

void ModuleEditorCamera::SetAspectRatio(float ratio)
{
	Frustum.SetVerticalFovAndAspectRatio(Frustum.VerticalFov(), ratio);
}

void ModuleEditorCamera::SetPlaneDistances(float near, float far)
{
	Frustum.SetViewPlaneDistances(near, far);
}

float3 ModuleEditorCamera::Position() const
{
	return Frustum.Pos();
}

float3 ModuleEditorCamera::Orientation() const
{
	return Frustum.Front();
}

void ModuleEditorCamera::LookAt(float x, float y, float z)
{
	
}

float* ModuleEditorCamera::GetProjectionMatrix() const
{
	float4x4 proj;
	
	proj = Frustum.ProjectionMatrix();
	proj.Transpose();

	return reinterpret_cast<float*>(proj.v);
}

float* ModuleEditorCamera::GetViewMatrix() const
{
	float4x4 view;
	
	view = Frustum.ComputeViewMatrix();
	view.Transpose();

	return reinterpret_cast<float*>(view.v);
}
