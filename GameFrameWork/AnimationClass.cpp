#include "AnimationClass.h"

AnimationClass::AnimationClass() {
	m_CurrentIndex = 1;
	m_LastUpdateAnim = std::chrono::system_clock::now();
}

AnimationClass::~AnimationClass() {
}

void AnimationClass::Init(TextureClass* Texture, int Width, int Height) {
	RECT Rect = Texture->GetRect();
	for (int i = 0; i < Height * Width; i++) {
		int Row = i / Width;
		int Col = i % Width;
		m_ImageRects.push_back(RECT{ LONG(Rect.right / Width) * Col, LONG(Rect.bottom / Height) * Row, LONG(Rect.right / Width) * (Col + 1), LONG(Rect.bottom / Height) * (Row + 1) });
	}
	if (!m_ImageRects.empty()) {
		Texture->m_ImageRect = m_ImageRects.front();
		Texture->SetImageCenter(D3DXVECTOR3(FLOAT(Texture->GetRect().right / 2), FLOAT(Texture->GetRect().bottom / 2), 0.f));
	}
}

bool AnimationClass::Update(const bool& bIsActivated, TextureClass* Texture) {
	if (bIsActivated && Texture && !m_ImageRects.empty()) {
		if (std::chrono::system_clock::now() - m_LastUpdateAnim > std::chrono::duration<float>(0.08f)) {
			if (m_CurrentIndex > m_ImageRects.size() - 1) {
				ClearAnimation();
				return true;
			}
			Texture->m_ImageRect = m_ImageRects[m_CurrentIndex++];
			m_LastUpdateAnim = std::chrono::system_clock::now();
		}
	}
	return false;
}

void AnimationClass::ClearAnimation() {
	m_CurrentIndex = 0;
	if (m_TriggerEventWhenAnimationEnd) {
		m_TriggerEventWhenAnimationEnd();
	}
}