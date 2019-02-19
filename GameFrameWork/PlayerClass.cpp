#include "PlayerClass.h"
#include "GraphicClass.h"
#include "SystemClass.h"
#include "InputClass.h"

PlayerClass::~PlayerClass() {
}

bool PlayerClass::Init(LPDIRECT3DDEVICE9 Device, LPCWSTR ImageSrc, bool bUseCustomRect, RECT CustomRect) {
	Actor::Init(Device, L"Player.png");

	SystemClass::GetInst()->GetInputClass()->BindAxisDelegate(VK_RIGHT, std::bind(&PlayerClass::MoveRight, this, 1.f));
	SystemClass::GetInst()->GetInputClass()->BindActionDelegate(VK_SPACE, IE_Pressed, std::bind(&PlayerClass::PopupMessageBox, this));
	return true;
}

void PlayerClass::Update() {

}

void PlayerClass::Render(LPD3DXSPRITE Sprite) {
	Actor::Render(Sprite);

}

void PlayerClass::Destroy() {

}

void PlayerClass::MoveRight(float Value) {
	m_Image->m_Position.x += Value * 2;
}

void PlayerClass::PopupMessageBox() {
	MessageBox(SystemClass::GetInst()->GetWindowHandle(), L"Pressed Space Bar!!", L"HI!", MB_OK);
}