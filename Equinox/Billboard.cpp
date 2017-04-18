#include "Billboard.h"
#include <GL/glew.h>
#include <MathGeoLib/include/Math/float3.h>
#include "Engine.h"

Billboard::Billboard(): _texture(0), _width(0.f), _height(0.f)
{
}


Billboard::~Billboard()
{
}

void Billboard::SetTexture(unsigned textureId)
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

void Billboard::Draw(const float3& position, const float3& up, const float3& right)
{
	if (_texture != 0)
	{
		glColor3f(1.f, 1.f, 1.f);
		glEnable(GL_BLEND);

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

void Billboard::GetSize(int& width, int& height)
{
	width = _width;
	height = _height;
}
