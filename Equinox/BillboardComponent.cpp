#include "BillboardComponent.h"
#include "TransformComponent.h"
#include <GL/glew.h>
#include "Engine.h"
#include "ModuleEditorCamera.h"
#include "MathGeoLib/include/Math/float3x3.h"

BillboardComponent::BillboardComponent()
{
	Name = "Billboard";
}


BillboardComponent::~BillboardComponent()
{
}

void BillboardComponent::Update()
{
	if (_texture != 0)
	{
		glColor3f(1.f, 1.f, 1.f);
		glEnable(GL_BLEND);

		float3 up, right;
		ComputeQuad(*App->editorCamera->GetCamera(), up, right);
		//right = float3::unitZ;

		float3 position = Parent->GetTransform()->Position;

		float halfX = (_height * 0.5f) / _height;
		float halfY = (_width * 0.5f) / _width;
		float3 vertex1 = (position + up * _height * 0.5f) + (right * _width * 0.5f);
		float3 vertex2 = (position - up * _height * 0.5f) - (right * _width * 0.5f);
		float3 vertex3 = (position - up * _height * 0.5f) + (right * _width * 0.5f);
		float3 vertex4 = (position + up * _height * 0.5f) - (right * _width * 0.5f);

		glBindTexture(GL_TEXTURE_2D, _texture);

		glBegin(GL_TRIANGLES);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3fv(reinterpret_cast<GLfloat*>(&vertex1));
		glTexCoord2f(1.0f, 0.0f);
		glVertex3fv(reinterpret_cast<GLfloat*>(&vertex2));
		glTexCoord2f(0.0f, 0.0f);
		glVertex3fv(reinterpret_cast<GLfloat*>(&vertex3));

		glTexCoord2f(0.0f, 1.0f);
		glVertex3fv(reinterpret_cast<GLfloat*>(&vertex1));
		glTexCoord2f(1.0f, 1.0f);
		glVertex3fv(reinterpret_cast<GLfloat*>(&vertex4));
		glTexCoord2f(1.0f, 0.0f);
		glVertex3fv(reinterpret_cast<GLfloat*>(&vertex2));
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_BLEND);
	}
}

void BillboardComponent::DrawUI()
{
}

void BillboardComponent::SetTexture(unsigned textureId)
{
	_texture = textureId;

	glBindTexture(GL_TEXTURE_2D, textureId);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &_width);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &_height);
	float ratio = _width / _height;
	_width = ratio;
	_height = 1;
	glBindTexture(GL_TEXTURE_2D, 0);
}

void BillboardComponent::ComputeQuad(const CameraComponent& camera, float3& up, float3& right) const
{
	TransformComponent* transform = Parent->GetTransform();

	if (transform)
	{
		float3 position = transform->Position;
		up = float3::unitY;
		right = (position - camera.Position()).Normalized().Cross(up);
		right.Normalize();
	}
}
