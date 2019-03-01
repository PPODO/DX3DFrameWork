#include "PlayerClass.h"

PlayerClass::PlayerClass() {
}

PlayerClass::~PlayerClass() {
}

bool PlayerClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	Pawn::Init(Device, L"Player.png");

	return true;
}

void PlayerClass::SetupPlayerInput() {
}

void PlayerClass::Update(float DeltaTime) {
	Pawn::Update(DeltaTime);
}

void PlayerClass::Render(LPD3DXSPRITE Sprite) {
	Pawn::Render(Sprite);
}

void PlayerClass::Destroy() {
	Pawn::Destroy();
}