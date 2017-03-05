#include "TransformComponent.h"
#include <GL/glew.h>

TransformComponent::TransformComponent()
{
	Name = "Transform";
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::Update()
{	
	glTranslatef(Position.x, Position.y, Position.z);
	float3 rotation = Rotation.ToEulerXYZ();
	glRotatef(Rotation.Angle(), rotation.x, rotation.y, rotation.z);
	glScalef(Scale.x, Scale.y, Scale.z);
}
