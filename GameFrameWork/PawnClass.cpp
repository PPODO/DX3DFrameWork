#include "PawnClass.h"
#include "SystemClass.h"
#include "ObjectPoolClass.h"

RECT PawnClass::m_WindowSize = { 0,0,0,0 };

PawnClass::PawnClass() : m_bStartMoveToLocation(true), m_bUseGravity(true), m_bLanded(true), m_bIsJumping(false), m_SaveYLocation(0.f), m_CurrentHeight(0.f), m_MaxHeight(-10.f), m_LocationIsHaveToGo(0.f) {
	m_WindowSize = SystemClass::GetInst()->GetWindowSize();
	m_bIsActivated = true;
	m_bIsFire = false;
	m_ProjectileType = EPT_DEFAULT;
	m_PoolManager = nullptr;
}

PawnClass::~PawnClass() {
	
}

bool PawnClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	Actor::Init(Device, FileSrc);
	SetupPlayerInput();

	return true;
}

void PawnClass::Update(float DeltaTime) {
	Actor::Update(DeltaTime);

	if (m_bStartMoveToLocation) {
		PlayStartMoveToLocation();
	}
	else {
		CalculateJump();
	}
}

void PawnClass::Render(LPD3DXSPRITE Sprite) {
	Actor::Render(Sprite);
}