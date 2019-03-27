#include "AnimationClass.h"

AnimationClass::AnimationClass() {
	m_CurrentIndex = 1;
	m_LastUpdateAnim = std::chrono::system_clock::now();
}

AnimationClass::~AnimationClass() {
}

void AnimationClass::Init(TextureClass* Texture) {
	RECT Rect = Texture->GetRect();
	for (int i = 0; i < ParticleHeight * ParticleWidth; i++) {
		int Row = i / ParticleWidth;
		int Col = i % ParticleWidth;
		m_ImageRects.push_back(RECT{ LONG(Rect.right / ParticleWidth) * Col, LONG(Rect.bottom / ParticleHeight) * Row, LONG(Rect.right / ParticleWidth) * (Col + 1), LONG(Rect.bottom / ParticleHeight) * (Row + 1) });
	}
	Texture->m_ImageRect = m_ImageRects.front();
	Texture->SetImageCenter(D3DXVECTOR3(FLOAT(Texture->GetRect().right / 2), FLOAT(Texture->GetRect().bottom / 2), 0.f));
}

bool AnimationClass::Update(const bool& bIsActivated, TextureClass* Texture) {
	if (bIsActivated && Texture) {
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