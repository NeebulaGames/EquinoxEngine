#include "ModuleEditorCamera.h"
#include "Engine.h"
#include "ModuleInput.h"
#include <MathGeoLib/include/Math/Quat.h>

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

update_status ModuleEditorCamera::Update()
{
	float3 movement = float3::zero;
	
	float rotateUp = 0;
	float rotateRight = 0;

	if (App->input->GetKey(SDL_SCANCODE_UP))
		rotateUp += 1;
	if (App->input->GetKey(SDL_SCANCODE_DOWN))
		rotateUp -= 1;
	if (App->input->GetKey(SDL_SCANCODE_RIGHT))
		rotateRight += 1;
	if (App->input->GetKey(SDL_SCANCODE_LEFT))
		rotateRight -= 1;

	if (rotateRight != 0)
	{
		Quat rot = Quat::RotateAxisAngle(Frustum.Up(), DegToRad(rotateRight));

		Frustum.SetFront(rot.Mul(Frustum.Front()).Normalized());
		Frustum.SetUp(rot.Mul(Frustum.Up()).Normalized());
	}

	if (rotateUp != 0.f)
	{
		Quat rot = Quat::RotateAxisAngle(Frustum.WorldRight(), DegToRad(rotateUp));

		float3 up = rot.Mul(Frustum.Up()).Normalized();

		if (up.y > 0.f)
		{
			Frustum.SetFront(rot.Mul(Frustum.Front()).Normalized());
			Frustum.SetUp(up);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_Q))
		movement += float3::unitY;
	if (App->input->GetKey(SDL_SCANCODE_E))
		movement -= float3::unitY;
	if (App->input->GetKey(SDL_SCANCODE_W))
		movement += Frustum.Front();
	if (App->input->GetKey(SDL_SCANCODE_S))
		movement -= Frustum.Front();
	if (App->input->GetKey(SDL_SCANCODE_A))
		movement -= Frustum.WorldRight();
	if (App->input->GetKey(SDL_SCANCODE_D))
		movement += Frustum.WorldRight();

	Frustum.SetPos(Frustum.Pos() + movement);

	return UPDATE_CONTINUE;
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
	static float4x4 proj;
	
	proj = Frustum.ProjectionMatrix();
	proj.Transpose();

	return reinterpret_cast<float*>(proj.v);
}

float* ModuleEditorCamera::GetViewMatrix() const
{
	static float4x4 view;

	view = Frustum.ViewMatrix();

	view.Transpose();

	return reinterpret_cast<float*>(view.v);
}
