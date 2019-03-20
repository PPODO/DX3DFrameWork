#include "ButtonClass.h"
#include "SystemClass.h"
#include "InputClass.h"
#include "StageClass.h"

ButtonClass::ButtonClass(LPCTSTR PressedImage, LPCTSTR ReleaseImage) {
	SetDrawImageIndex(EBS_Released);
	m_FileSrc.push_back(PressedImage);
	m_FileSrc.push_back(ReleaseImage);
}

ButtonClass::~ButtonClass() {
}

bool ButtonClass::Init(LPDIRECT3DDEVICE9 Device, const std::vector<LPCTSTR>& FileSrc) {
	WidgetClass::Init(Device, FileSrc);

	TextureClass* ButtonTexture = GetTextureByIndex(EBS_Pressed);
	if (!ButtonTexture || ButtonTexture->m_Texture) {
		return false;
	}
	SystemClass::GetInst()->GetInputManager()->BindMouseActionDelegate(EST_MAIN, ButtonTexture->GetRect(), std::bind(&ButtonClass::ChangeButtonState, this), []() {});

	return true;
}

void ButtonClass::ChangeButtonState() {
	SetDrawImageIndex(!GetDrawImageIndex());
}