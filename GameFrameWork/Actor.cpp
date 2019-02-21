#include "Actor.h"
#include "TextureClass.h"

Actor::Actor(std::vector<Actor*>& Vector) {
	Vector.push_back(this);
}

bool Actor::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	m_Texture = (CustomRect.left < 0 ? new TextureClass(Device, FileSrc) : new TextureClass(Device, FileSrc, CustomRect));
	if (!m_Texture) {
		return false;
	}

	return true;
}

void Actor::Render(LPD3DXSPRITE Sprite) {
	Sprite->Draw(m_Texture->m_Texture, &m_Texture->m_ImageRect, &m_Texture->m_ImageCenter, &m_Texture->m_ImagePosition, m_Texture->m_Color);
}