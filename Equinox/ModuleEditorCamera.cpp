#include "ModuleEditorCamera.h"
#include "Engine.h"
#include "ModuleInput.h"
#include <MathGeoLib/include/Math/Quat.h>

using namespace math;

ModuleEditorCamera::ModuleEditorCamera()
{
	_cameraComponent = new ::CameraComponent();
}

ModuleEditorCamera::~ModuleEditorCamera()
{
}


update_status ModuleEditorCamera::Update(float DeltaTime)
{
	float3 movement = float3::zero;
	
	float rotateUp = 0;
	float rotateRight = 0;
	SDL_ShowCursor(1);
	SDL_SetRelativeMouseMode(SDL_FALSE);
	if(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT))
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_ShowCursor(0);
		iPoint mouse_movement = App->input->GetMouseMotion();
		float2 mouse_drag = { float(mouse_movement.x), float(mouse_movement.y) };

		if(!mouse_drag.IsZero())
			mouse_drag.Normalize();
		
		rotateUp = -mouse_drag.y * 3.f;
		rotateRight = -mouse_drag.x * 3.f;
	}

	if (App->input->GetKey(SDL_SCANCODE_UP))
		rotateUp += 1;
	if (App->input->GetKey(SDL_SCANCODE_DOWN))
		rotateUp -= 1;
	if (App->input->GetKey(SDL_SCANCODE_RIGHT))
		rotateRight -= 1;
	if (App->input->GetKey(SDL_SCANCODE_LEFT))
		rotateRight += 1;

	CLAMP(rotateUp, -1, 1);
	CLAMP(rotateRight, -1, 1);

	if (rotateRight != 0)
	{
		Quat rot = Quat::RotateY(DegToRad(rotateRight));

		//_frustum.SetFront(rot.Mul(_frustum.Front()).Normalized());
		_cameraComponent->SetFront(rot.Mul(_cameraComponent->Orientation()).Normalized());
		_cameraComponent->SetUp(rot.Mul(_cameraComponent->GetUp()).Normalized());
	}

	if (rotateUp != 0.f)
	{
		Quat rot = Quat::RotateAxisAngle(_cameraComponent->GetWorldRight(), DegToRad(rotateUp));

		float3 up = rot.Mul(_cameraComponent->GetUp()).Normalized();

		if (up.y > 0.f)
		{
			_cameraComponent->SetFront(rot.Mul(_cameraComponent->Orientation()).Normalized());
			_cameraComponent->SetUp(up);
		}
	}

	iPoint wheel_movement = App->input->GetMouseWheel();

	if(wheel_movement.y != 0)
	{
		if(wheel_movement.y > 0)
			movement += _cameraComponent->Orientation();
		else
			movement -= _cameraComponent->Orientation();
	}

	if (App->input->GetKey(SDL_SCANCODE_Q))
		movement += float3::unitY;
	if (App->input->GetKey(SDL_SCANCODE_E))
		movement -= float3::unitY;
	if (App->input->GetKey(SDL_SCANCODE_W))
		movement += _cameraComponent->Orientation();
	if (App->input->GetKey(SDL_SCANCODE_S))
		movement -= _cameraComponent->Orientation();
	if (App->input->GetKey(SDL_SCANCODE_A))
		movement -= _cameraComponent->GetWorldRight();
	if (App->input->GetKey(SDL_SCANCODE_D))
		movement += _cameraComponent->GetWorldRight();

	float velocity = (App->input->GetKey(SDL_SCANCODE_LSHIFT) || wheel_movement.y != 0)? 0.6f : 0.1f;

	_cameraComponent->SetPos(_cameraComponent->Position() + movement*velocity);

	return UPDATE_CONTINUE;
}

bool ModuleEditorCamera::CleanUp()
{
	RELEASE(_cameraComponent);
	return true;
}

void ModuleEditorCamera::SetFOV(float fov) const
{
	_cameraComponent->SetFOV(fov);
}

void ModuleEditorCamera::SetAspectRatio(float ratio) const
{
	_cameraComponent->SetAspectRatio(ratio);
}

void ModuleEditorCamera::SetPlaneDistances(float near, float far) const
{
	_cameraComponent->SetPlaneDistances(near, far);
}

float3 ModuleEditorCamera::Position() const
{
	return _cameraComponent->Position();
}

float3 ModuleEditorCamera::Orientation() const
{
	return _cameraComponent->Orientation();
}

void ModuleEditorCamera::LookAt(float x, float y, float z) const
{
	_cameraComponent->LookAt(x, y, z);
}

float* ModuleEditorCamera::GetProjectionMatrix() const
{
	return _cameraComponent->GetProjectionMatrix();
}

float* ModuleEditorCamera::GetViewMatrix() const
{
	return _cameraComponent->GetViewMatrix();
}

CameraComponent* ModuleEditorCamera::GetCamera() const
{
	return _cameraComponent;
}

bool ModuleEditorCamera::FrustumCollingEnabled() const
{
	return _cameraComponent->FrustumCullingEnabled;
}
