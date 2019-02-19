#include "PlayerClass.h"
#include "GraphicClass.h"
#include "SystemClass.h"
#include "InputClass.h"

PlayerClass::PlayerClass() {
}

PlayerClass::~PlayerClass() {
}

bool PlayerClass::Init(LPDIRECT3DDEVICE9 Device, LPCWSTR ImageSrc, bool bUseCustomRect, RECT CustomRect) {
	Actor::Init(Device, L"Player.png");

	SystemClass::GetInst()->GetInputClass()->BindDelegate(VK_RIGHT, std::bind(&PlayerClass::MoveRight, this, 1.f));
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