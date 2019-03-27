#include "Actor.h"
#include "SystemClass.h"

RECT Actor::m_WindowSize;

Actor::Actor() : m_CollisionType(ECT_NONE), m_bIsActivated(false), m_bIsTimerLoop(false), m_bIsUseTimer(false) {
	m_WindowSize = SystemClass::GetInst()->GetWindowSize();
}

Actor::~Actor() {
	ClearObject();
	if (m_Image) {
		delete m_Image;
		m_Image = nullptr;
	}
}

bool Actor::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	m_Image = new TextureClass(Device, FileSrc);
	if (!m_Image || !m_Image->m_Texture) {
		return false;
	}

	return true;
} 

void Actor::Update(float DeltaTime, float ActorHeight) {
	if (m_bIsUseTimer) {
		ProcessingTimer();
	}
	if (!m_bIsActivated) {
		return;
	}

	if (m_Image) {
		m_Image->AddYPosition(ActorHeight);
	}
}

void Actor::Render(LPD3DXSPRITE Sprite) {
	if (!m_bIsActivated) {
		return;
	}

	if (m_Image) {
		m_Image->Render(Sprite);
	}
}

bool Actor::IsItOutOfScreen() {
	if (m_Image->GetPosition().x - m_Image->GetImageCenter().x <= 0.f || m_Image->GetPosition().x + m_Image->GetImageCenter().x > m_WindowSize.right || m_Image->GetPosition().y - m_Image->GetImageCenter().y <= 0.f || m_Image->GetPosition().y + m_Image->GetImageCenter().y > m_WindowSize.bottom) {
		return true;
	}
	return false;
}

void Actor::SpawnActor(const D3DXVECTOR3& Location) {
	
}

void Actor::ClearObject() {
	
}