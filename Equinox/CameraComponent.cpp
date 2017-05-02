#include "CameraComponent.h"
#include "GameObject.h"
#include "MathGeoLib/include/Geometry/Plane.h"

CameraComponent::CameraComponent()
{
	Name = "Camera";

	_frustum.SetPos(float3::zero);
	_frustum.SetFront(float3::unitZ);
	_frustum.SetUp(float3::unitY);

	SetPlaneDistances(0.1f, 1000.0f);

	_frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);

	_frustum.SetVerticalFovAndAspectRatio(DegToRad(60), 1.f);
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Update(float dt)
{
}

void CameraComponent::SetFOV(float fov)
{
	_frustum.SetVerticalFovAndAspectRatio(fov, _frustum.AspectRatio());
}

void CameraComponent::SetAspectRatio(float ratio)
{
	_frustum.SetVerticalFovAndAspectRatio(_frustum.VerticalFov(), ratio);
}

void CameraComponent::SetPlaneDistances(float near, float far)
{
	_frustum.SetViewPlaneDistances(near, far);
}

float3 CameraComponent::Position() const
{
	return _frustum.Pos();
}

float3 CameraComponent::Orientation() const
{
	return _frustum.Front();
}

void CameraComponent::LookAt(float x, float y, float z)
{
}

float* CameraComponent::GetProjectionMatrix() const
{
	static float4x4 proj;

	proj = _frustum.ProjectionMatrix();
	proj.Transpose();

	return reinterpret_cast<float*>(proj.v);
}

float* CameraComponent::GetViewMatrix() const
{
	static float4x4 view;

	view = _frustum.ViewMatrix();

	view.Transpose();

	return reinterpret_cast<float*>(view.v);
}

void CameraComponent::SetFront(const vec& front)
{
	_frustum.SetFront(front);
}

void CameraComponent::SetUp(const vec& up)
{
	_frustum.SetUp(up);
}

void CameraComponent::SetPos(const vec& pos)
{
	_frustum.SetPos(pos);
}

const vec& CameraComponent::GetUp() const
{
	return _frustum.Up();
}

vec CameraComponent::GetWorldRight() const
{
	return _frustum.WorldRight();
}

AABB CameraComponent::GetFrustumAABB()
{
	return _frustum.MinimalEnclosingAABB();
}

//void CameraComponent::CheckInsideFrustrum(std::vector<GameObject*> objectsInScene)
//{
//	for (GameObject* go : objectsInScene)
//	{
//		if (containsAABB(go))
//			go->VisibleOnCamera = true;
//		else
//			go->VisibleOnCamera = false;
//	}
//}

bool CameraComponent::containsAABB(GameObject* go)
{

	// TODO: CHECK FRUSTUM CULLING
	vec vCorner[8];
	int iTotalIn = 0;
	go->BoundingBox.GetCornerPoints(vCorner);

	Plane frustumPlanes[6];
	_frustum.GetPlanes(frustumPlanes);

	// test 8 corners against 6 sides
	for (int p = 0; p < 6; ++p) {
		int iInCount = 8;
		int iPtIn = 1;
		for (int i = 0; i < 8; ++i) {
			if (frustumPlanes[i].SignedDistance(vCorner[i]) > 0)
			{
				iPtIn = 0;
				--iInCount;
			}
		}
		if (iInCount == 0)
			return false; // all paints outside plane p
		iTotalIn += iPtIn;
	}
	if (iTotalIn == 6)
		return true;
	return false;
}
