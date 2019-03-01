#include "ButtonClass.h"
#include "SystemClass.h"
#include "TextureClass.h"
#include "InputClass.h"

ButtonClass::ButtonClass(unsigned short CurrentStage) : m_bChangeButtonState(false), m_CurrentStage(CurrentStage) {
}

bool ButtonClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, std::function<void()> Work) {
	Actor::Init(Device, FileSrc);

	RECT ButtonRect = RECT{ (LONG)m_Texture->m_ImagePosition.x + m_Texture->m_ImageRect.left, (LONG)m_Texture->m_ImagePosition.y + m_Texture->m_ImageRect.top, (LONG)m_Texture->m_ImagePosition.x + m_Texture->m_ImageRect.right, (LONG)m_Texture->m_ImagePosition.y + m_Texture->m_ImageRect.bottom };
	SystemClass::GetInst()->GetInputManager()->BindMouseActionDelegate(m_CurrentStage, ButtonRect, std::bind(&ButtonClass::ChangeButtonState, this), Work);
	return true;
}

void ButtonClass::ChangeButtonState() {
	m_bChangeButtonState = !m_bChangeButtonState;
	if (m_bChangeButtonState) {
		m_Texture->m_Color = D3DXCOLOR(0, 255, 255, 1);
	}
	else {
		m_Texture->m_Color = 0xffffffff;
	}
}