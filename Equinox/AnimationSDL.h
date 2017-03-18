#ifndef __ANIMATIONSDL_H__
#define __ANIMATIONSDL_H__

#include <vector>
#include "Sprite.h"

using namespace std;

template<typename T>
class AnimationSDL
{
public:
	bool loop = true;
	float speed = 1.0f;
	vector<T> frames;

private:
	float current_frame = 0.0f;
	int loops = 0;

public:
	AnimationSDL()
	{}

	AnimationSDL(const AnimationSDL& anim) : loop(anim.loop), speed(anim.speed), frames(anim.frames)
	{}

	T& GetCurrentFrame()
	{
		float last_frame = (float) frames.size();

		current_frame += speed;
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : MAX(last_frame - 1.0f, 0.0f);
			loops++;
		} 

		return frames[(int)current_frame];
	}

	float GetCurrentFrameNumber() const
	{
		return current_frame;
	}

	void SetFrame(float frame)
	{
		if (uint(frame) < frames.size())
			current_frame = frame;
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0.0f;
	}
};

typedef AnimationSDL<SDL_Rect> RectAnimation;
typedef AnimationSDL<Sprite> SpriteAnimation;

#endif // __ANIMATIONSDL_H__
