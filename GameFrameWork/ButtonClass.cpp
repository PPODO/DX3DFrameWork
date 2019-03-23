#include "ButtonClass.h"
#include "SystemClass.h"
#include "InputClass.h"
#include "StageClass.h"

ButtonClass::ButtonClass(LPCTSTR PressedImage) {
	m_FileSrc.push_back(PressedImage);
}

ButtonClass::~ButtonClass() {
}

bool ButtonClass::Init(LPDIRECT3DDEVICE9 Device, const std::vector<LPCTSTR>& FileSrc) {
	WidgetClass::Init(Device, m_FileSrc);

	TextureClass* ButtonTexture = GetTextureByIndex(0);
	if (!ButtonTexture || !ButtonTexture->m_Texture) {
		return false;
	}
	return true;
}
void ButtonClass::BindMouseAction(const size_t& EST, const std::function<void()>& Func) {
	TextureClass* ButtonTexture = GetTextureByIndex(0);
	if (ButtonTexture) {
		RECT HitRect{ LONG(ButtonTexture->GetPosition().x) + ButtonTexture->GetRect().left, LONG(ButtonTexture->GetPosition().y) + ButtonTexture->GetRect().top, LONG(ButtonTexture->GetPosition().x) + ButtonTexture->GetRect().right, LONG(ButtonTexture->GetPosition().y) + ButtonTexture->GetRect().bottom };
		SystemClass::GetInst()->GetInputManager()->BindMouseActionDelegate(EST, HitRect, std::bind(&ButtonClass::ChangeButtonState, this), Func);
	}
}

void ButtonClass::ChangeButtonState() {

}