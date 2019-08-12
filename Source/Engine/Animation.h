#pragma once
#include <vector>
#include "Application.h"
#include "./Modules/ModuleTime.h"
#include <SDL.h>

using namespace std;

class Animation
{
public:
	bool loop = true;
	bool randFrame = false;
	bool timeBased = true;
	bool inversed = false;
	float speed = 1.0f;
	vector<SDL_Rect> frames;

private:
	int current_frame = 0;
	int loops = 0;
	float timer=0;
	bool first = true;

public:
	Animation()
	{}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), frames(anim.frames),randFrame(anim.randFrame)
	{}

	void Update() {
		if (first) {
			if (randFrame) {
				current_frame = RAND() % frames.size();
			}
			if (!timeBased) {
				timer = App->time->GetTimeSinceStart() + (1 / speed);
			}
			first = false;
		}

		int last_frame = frames.size();

		if (timeBased) {
			if (timer >= 1) {
				if (randFrame) {
					current_frame = RAND() % frames.size();
				}
				else
				{
					if (inversed) {
						--current_frame;
					}
					else
					{
						++current_frame;
					}
				}
				timer = 0;
			}
			else
			{
				timer += speed*App->time->GetDeltaTime();
			}
		}
		else {
			if (timer >= 1) {
				if (randFrame) {
					current_frame = RAND() % frames.size();
				}
				else
				{
					if (inversed) {
						--current_frame;
					}
					else
					{
						++current_frame;
					}
				}
				timer = 0;
			}
			else
			{
				timer += speed*App->time->GetUnscaledDeltaTime();
			}
		}

		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0 : MAX(last_frame - 1, 0);
			loops++;
		}

		if (current_frame < 0) {
			current_frame = (loop) ? MAX(last_frame - 1, 0) : 0;
			loops++;
		}
	}

	SDL_Rect& GetCurrentFrame()
	{
		return frames[current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		if (inversed) {
			current_frame = MAX(frames.size()-1,0);
		}
		else {
			current_frame = 0;
		}
		loops = 0;
		timer = 0;
		first = true;
	}

	const SDL_Rect& GetCurrentFrameConst() const{
		return frames[current_frame];
	}

	void SetNextFrame() {
		if (inversed) {
			current_frame = (current_frame - 1)+frames.size() % frames.size();
		}
		else {
			current_frame = (current_frame + 1) % frames.size();
		}
	}

	void SetPreviousFrame() {
		if (inversed) {
			current_frame = (current_frame + 1) % frames.size();
		}
		else {
			current_frame = (current_frame - 1) + frames.size() % frames.size();
		}
	}

	int GetCurrentFrameIndex()const {
		return current_frame;
	}
};