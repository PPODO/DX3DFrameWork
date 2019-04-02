#pragma once
#include "TextureClass.h"
#include <functional>
#include <vector>
#include <chrono>

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
	void Init(TextureClass* Texture, int Width, int Height);
	bool Update(const bool& bIsActivated, TextureClass* Texture);
	void ClearAnimation();

public:
	void BindEndNotification(const std::function<void()>& Func) {
		m_TriggerEventWhenAnimationEnd = Func;
	}

};