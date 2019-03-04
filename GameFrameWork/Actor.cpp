#include "Actor.h"
#include "SystemClass.h"
#include "TextureClass.h"

RECT Actor::m_WindowSize;

Actor::Actor() : m_bIsActive(true), m_Texture(nullptr) {
	m_WindowSize = SystemClass::GetInst()->GetWindowSize();
}

bool Actor::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	m_Texture = (CustomRect.left <= -1 ? new TextureClass(Device, FileSrc) : new TextureClass(Device, FileSrc, CustomRect));
	if (!m_Texture) {
		return false;
	}

	return true;
}

void Actor::Update(float DeltaTime) {
	if (!m_bIsActive) { return; }
}

void Actor::Render(LPD3DXSPRITE Sprite) {
	if (!m_bIsActive) { return; }

	if (m_Texture) {
		m_Texture->Render(Sprite);
	}
}

void Actor::Destroy() {
	if (m_Texture) {
		delete m_Texture;
		m_Texture = nullptr;
	}
}

bool Actor::IsItOutOfScreen() {
	// 화면을 나갔는지 검사해주는 함수 부분 EventClass에서 사용됨.
	if (m_Texture->GetPosition().x + m_Texture->GetImageCenter().x < GetWindowSize().left || m_Texture->GetPosition().x - m_Texture->GetImageCenter().x > GetWindowSize().right || m_Texture->GetPosition().y + m_Texture->GetImageCenter().y < GetWindowSize().top || m_Texture->GetPosition().y - m_Texture->GetImageCenter().y > GetWindowSize().bottom) {
		return true;
	}
	return false;
}

void Actor::OutOfScreen() {
}

void Actor::TriggerCollisionEventByOtherActor(Actor *) {
}