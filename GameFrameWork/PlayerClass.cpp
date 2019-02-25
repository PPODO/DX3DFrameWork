#include "PlayerClass.h"
#include "SystemClass.h"
#include "InputClass.h"

PlayerClass::PlayerClass() {
	m_XMoveSpeed = 5.f;
	m_YMoveSpeed = 5.f;
}

bool PlayerClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	Pawn::Init(Device, L"Player.png");
	m_Texture->SetPosition(m_Texture->GetImageCenter());

	return true;
}

void PlayerClass::SetupPlayerInput() {
	SystemClass::GetInst()->GetInputManager()->BindAxisDelegate(VK_RIGHT, std::bind(&PlayerClass::MoveRight, this, 1.f));
	SystemClass::GetInst()->GetInputManager()->BindAxisDelegate(VK_LEFT, std::bind(&PlayerClass::MoveRight, this, -1.f));
	SystemClass::GetInst()->GetInputManager()->BindAxisDelegate(VK_UP, std::bind(&PlayerClass::MoveForward, this, -1.f));
	SystemClass::GetInst()->GetInputManager()->BindAxisDelegate(VK_DOWN, std::bind(&PlayerClass::MoveForward, this, 1.f));
}

void PlayerClass::Update(float DeltaTime) {

}

void PlayerClass::Render(LPD3DXSPRITE Sprite) {
	Pawn::Render(Sprite);

}

void PlayerClass::Destroy() {
	Pawn::Destroy();

}

void PlayerClass::MoveRight(float Value) {
	if (CheckOutOfScreen(XSCREEN, (LONG)Value)) {
		m_Texture->AddXPosition(Value * m_XMoveSpeed);
	}
}

void PlayerClass::MoveForward(float Value) {
	if (CheckOutOfScreen(YSCREEN, (LONG)Value)) {
		m_Texture->AddYPosition(Value * m_YMoveSpeed);
	}
}