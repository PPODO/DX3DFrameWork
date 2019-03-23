#pragma once
#include "TextureClass.h"
#include <vector>

class WidgetClass {
private:
	std::vector<TextureClass*> m_Images;
	size_t m_DrawImageIndex;

public:
	WidgetClass();
	virtual ~WidgetClass();

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, const std::vector<LPCTSTR>& FileSrc);
	virtual void Render(LPD3DXSPRITE Sprite);

public:
	void SetDrawImageIndex(size_t Index) { m_DrawImageIndex = Index; }
	void SetWidgetLocation(const D3DXVECTOR3& Pos) { for (auto& It : m_Images) { if (It) { It->SetPosition(Pos); } } };
	size_t GetDrawImageIndex() const { return m_DrawImageIndex; }
	TextureClass* GetTextureByIndex(size_t Index) { if (Index < m_Images.size()) { return m_Images[Index]; } return nullptr; };

};