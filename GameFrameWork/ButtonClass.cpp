#include "ButtonClass.h"
#include "SystemClass.h"
#include "TextureClass.h"
#include "InputClass.h"

ButtonClass::ButtonClass(unsigned short CurrentStage) : m_bChangeButtonState(false), m_CurrentStage(CurrentStage) {
	m_Work = 0;
}

bool ButtonClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, std::function<void()> Work) {
	Actor::Init(Device, FileSrc);

	m_Work = Work;
	return true;
}

void ButtonClass::ChangeButtonState() {
	m_bChangeButtonState = !m_bChangeButtonState;
	if (m_bChangeButtonState) {
		m_Texture->m_Color = D3DXCOLOR(255, 0, 255, 1);
	}
	else {
		m_Texture->m_Color = 0xffffffff;
	}
}

void ButtonClass::BindButtonAction() {
	D3DXVECTOR3 ButtonPosition = m_Texture->GetPosition() - m_Texture->GetImageCenter();
	RECT ButtonRect = RECT{ (LONG)ButtonPosition.x - m_Texture->m_ImageRect.left, (LONG)ButtonPosition.y - m_Texture->m_ImageRect.top, (LONG)ButtonPosition.x + m_Texture->m_ImageRect.right, (LONG)ButtonPosition.y + m_Texture->m_ImageRect.bottom };
	SystemClass::GetInst()->GetInputManager()->BindMouseActionDelegate(m_CurrentStage, ButtonRect, std::bind(&ButtonClass::ChangeButtonState, this), m_Work);
}