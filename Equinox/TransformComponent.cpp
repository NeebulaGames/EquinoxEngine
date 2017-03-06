#include "TransformComponent.h"
#include <GL/glew.h>
#include <MathGeoLib/include/Math/float4x4.h>

TransformComponent::TransformComponent()
{
	Name = "Transform";
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::Update()
{	
	float4x4 transformMatrix = float4x4::FromTRS(Position, Rotation, Scale);
	glMultMatrixf(reinterpret_cast<GLfloat*>(&transformMatrix));
}
