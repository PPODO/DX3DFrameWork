#include "Actor.h"
#include "TextureClass.h"

Actor::Actor() : m_bIsActive(true), m_Texture(nullptr) {
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