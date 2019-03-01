#include "Pawn.h"
#include "SystemClass.h"
#include "ProjectileClass.h"

Pawn::Pawn() : m_XMoveSpeed(0.f), m_YMoveSpeed(0.f), m_FireDelay(0.f), m_CurrentActivatedProjectile(0), m_MaxActivatedProjectile(0), m_bIsMaximallyActive(false), m_CurrentProjectileStyle(PS_DEFAULT) {
	m_WindowSize = SystemClass::GetInst()->GetWindowSize();
	m_LastFireTime = std::chrono::system_clock::now();
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
	Actor::Update(DeltaTime);
	
	if (m_bIsMaximallyActive) {
		if (m_CurrentActivatedProjectile < m_MaxActivatedProjectile) {
			m_bIsMaximallyActive = false;
		}
	}
}

void Pawn::Render(LPD3DXSPRITE Sprite) {
	Actor::Render(Sprite);

	for (auto Iterator : m_ActivedProjectiles) {
		if (Iterator->GetIsActivation()) {
			Iterator->Render(Sprite);
		}
	}
}

void Pawn::Destroy() {
	if (m_Texture) {
		delete m_Texture;
		m_Texture = nullptr;
	}
}