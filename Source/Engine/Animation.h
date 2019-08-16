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
	int m_currentFrame = 0;
	int m_loops = 0;
	float m_timer=0;

public:
	Animation()
	{}

	Animation(const Animation& anim) : loop(anim.loop), randFrame(anim.randFrame), timeBased(anim.timeBased),
	inversed(anim.inversed), speed(anim.speed), frames(anim.frames)
	{}

	void Start()
	{
		Reset();
	}

	void Update() {
		if(m_timer >= 1)
		{
			SetNextFrame();
			m_timer = 0;
		}
		
		if(timeBased)
		{
			m_timer += speed * Time->GetDeltaTime();
		}
		else
		{
			m_timer += speed * Time->GetUnscaledDeltaTime();
		}
		
	}

	SDL_Rect& GetCurrentFrame()
	{
		return frames[m_currentFrame];
	}

	bool Finished() const
	{
		return m_loops > 0;
	}

	void Reset()
	{
		if(randFrame)
		{
			m_currentFrame = RAND() % frames.size();
		}
		else if(inversed)
		{
			m_currentFrame = MAX(frames.size() - 1, 0);
		}
		else
		{
			m_currentFrame = 0;
		}
		//TODO: Verify
		//if(!timeBased)
		//{
		//	m_timer = Time->GetTimeSinceStart() + (1 / speed);
		//}
		m_loops = 0;
		m_timer = 0;
	}

	const SDL_Rect& GetCurrentFrameConst() const{
		return frames[m_currentFrame];
	}

	void SetNextFrame() {
		if(randFrame)
		{
			m_currentFrame = RAND() % frames.size();
		}
		else
		{
			if(inversed)
			{
				--m_currentFrame;
			}
			else
			{
				++m_currentFrame;
			}
			VerifyEndOfAnimation();
		}
	}

	void SetPreviousFrame() {
		if(randFrame)
		{
			m_currentFrame = RAND() % frames.size();
		}
		else
		{
			if(inversed)
			{
				++m_currentFrame;
			}
			else
			{
				--m_currentFrame;
			}
			VerifyEndOfAnimation();
		}
	}

	void VerifyEndOfAnimation()
	{
		int lastFrame = frames.size();
		if (m_currentFrame >= lastFrame)
		{
			m_currentFrame = (loop) ? 0 : MAX(lastFrame - 1, 0);
			m_loops++;
		}

		if (m_currentFrame < 0) {
			m_currentFrame = (loop) ? MAX(lastFrame - 1, 0) : 0;
			m_loops++;
		}
	}

	int GetCurrentFrameIndex()const {
		return m_currentFrame;
	}
};