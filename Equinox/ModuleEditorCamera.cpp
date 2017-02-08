#include "ModuleEditorCamera.h"

using namespace math;

ModuleEditorCamera::ModuleEditorCamera()
{
	Frustum.SetPos(float3::zero);
	Frustum.SetFront(float3::zero);
	Frustum.SetFront(float3(0.f, 0.f, -1.f));
	Frustum.SetUp(float3(0.f, 1.f, 0.f));

	SetPlaneDistances(1.f, 100.0f);

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
	float3x4 view;
	
	view = Frustum.ViewMatrix();
	view.Transpose3();

	return reinterpret_cast<float*>(view.v);
}
