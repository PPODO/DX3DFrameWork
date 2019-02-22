#include "PlayerClass.h"
#include "SystemClass.h"
#include "InputClass.h"
#include "TextureClass.h"

bool PlayerClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	Actor::Init(Device, L"Player.png", CustomRect);

	SystemClass::GetInst()->GetInputManager()->BindAxisDelegate(VK_RIGHT, std::bind(&PlayerClass::MoveRight, this, 1.f));

	return true;
}

void PlayerClass::Update(float DeltaTime) {

}

void PlayerClass::Destroy() {

}

void PlayerClass::MoveRight(float Value) {
	m_Texture->m_ImagePosition.x += Value;
}