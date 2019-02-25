#include "Pawn.h"
#include "SystemClass.h"

Pawn::Pawn() : m_XMoveSpeed(0.f), m_YMoveSpeed(0.f) {
	m_WindowSize = SystemClass::GetInst()->GetWindowSize();
}

Pawn::~Pawn() {
}

bool Pawn::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	Actor::Init(Device, FileSrc, CustomRect);
	
	SetupPlayerInput();
	return true;
}

void Pawn::SetupPlayerInput() {
}

void Pawn::Update(float DeltaTime) {
}

void Pawn::Render(LPD3DXSPRITE Sprite) {
	Actor::Render(Sprite);
}

void Pawn::Destroy() {
	if (m_Texture) {
		delete m_Texture;
		m_Texture = nullptr;
	}
}