#pragma once
#include "Actor.h"
#include <vector>

class TextClass : public Actor {
public:
	TextClass(std::vector<TextClass*>& m_Vectors);

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 });
	virtual void Render(LPD3DXSPRITE Sprite);
	virtual void Destroy();

};

