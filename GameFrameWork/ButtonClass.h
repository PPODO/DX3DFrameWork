#pragma once
#include "Actor.h"
#include <functional>

class ButtonClass : public Actor {
private:
	std::function<void()> m_Work;
	bool m_bChangeButtonState;
	unsigned short m_CurrentStage;

private:
	void ChangeButtonState();

public:
	ButtonClass(unsigned short);

	bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, std::function<void()> Work);

	void BindButtonAction();

public:
	class TextureClass* GetTexture() const { return m_Texture; }
};

