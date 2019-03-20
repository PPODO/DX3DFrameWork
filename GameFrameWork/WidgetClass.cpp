#include "WidgetClass.h"

WidgetClass::WidgetClass() : m_DrawImageIndex(0) {

}

WidgetClass::~WidgetClass() {
	for (auto& Iterator : m_Images) {
		if (Iterator || Iterator->m_Texture) {
			Iterator->m_Texture->Release();
			delete Iterator;
		}
	}
	m_Images.clear();
}

bool WidgetClass::Init(LPDIRECT3DDEVICE9 Device, const std::vector<LPCTSTR>& FileSrc) {
	for (const auto& Iterator : FileSrc) {
		TextureClass* Texture = new TextureClass(Device, Iterator);
		if (!Texture || !Texture->m_Texture) {
			return false;
		}
		Texture->SetImageCenter(D3DXVECTOR3(0.f, 0.f, 0.f));
		m_Images.push_back(Texture);
	}

	return true;
}

void WidgetClass::Render(LPD3DXSPRITE Sprite) {
	if (m_DrawImageIndex < m_Images.size() && m_Images[m_DrawImageIndex] && m_Images[m_DrawImageIndex]->m_Texture) {
		m_Images[m_DrawImageIndex]->Render(Sprite);
	}
}