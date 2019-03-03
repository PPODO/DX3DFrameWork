#include "TextClass.h"
#include "TextureClass.h"

TextClass::TextClass(std::vector<TextClass*>& m_Vectors) {
	m_Vectors.push_back(this);
	m_bIsActive = false;
}

bool TextClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	Actor::Init(Device, FileSrc, CustomRect);

	m_Texture->SetPosition(D3DXVECTOR3(GetWindowSize().right / 2, GetWindowSize().bottom / 2, 0.f));
	return true;
}

void TextClass::Render(LPD3DXSPRITE Sprite) {
	Actor::Render(Sprite);
}

void TextClass::Destroy() {
	Actor::Destroy();
}