#pragma once
#include "TextureClass.h"
#include <functional>
#include <vector>
#include <chrono>

static const int ParticleWidth = 4;
static const int ParticleHeight = 2;

class AnimationClass {
private:
	std::function<void()> m_TriggerEventWhenAnimationEnd;
	std::vector<RECT> m_ImageRects;
	size_t m_CurrentIndex;

private:
	std::chrono::system_clock::time_point m_LastUpdateAnim;


public:
	AnimationClass();
	~AnimationClass();

public:
	void Init(TextureClass* Texture);
	bool Update(const bool& bIsActivated, TextureClass* Texture);
	void ClearAnimation();

public:
	void BindEndNotification(const std::function<void()>& Func) {
		m_TriggerEventWhenAnimationEnd = Func;
	}

};