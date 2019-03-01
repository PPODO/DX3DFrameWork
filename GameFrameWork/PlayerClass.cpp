#include "PlayerClass.h"
#include "InputClass.h"
#include "SystemClass.h"
#include "DefaultProjectileClass.h"

PlayerClass::PlayerClass(ObjectPoolClass* OP) : m_TempInputManager(nullptr), m_TempPoolManager(OP) {
	m_Projectiles.resize(PS_COUNT);

	m_TempInputManager = SystemClass::GetInst()->GetInputManager();
	m_XMoveSpeed = 5.f;
	m_YMoveSpeed = 5.f;
	m_MaxActivatedProjectile = 10;
}

PlayerClass::~PlayerClass() {
}

bool PlayerClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	Pawn::Init(Device, L"Player.png");
	m_Texture->SetPosition(D3DXVECTOR3(0.f, FLOAT(GetWindowSize().bottom) / 2, 0.f) - m_Texture->GetImageCenter());

	if (!m_TempPoolManager) {
		return false;
	}
//	m_TempPoolManager->GetPoolObject("DefaultProjectile", m_Projectiles[PS_DEFAULT], 10);

	return true;
}

void PlayerClass::SetupPlayerInput() {
	if (!m_TempInputManager) {
		m_TempInputManager = SystemClass::GetInst()->GetInputManager();
	}

	m_TempInputManager->BindAxisDelegate(VK_RIGHT, std::bind(&PlayerClass::MoveRight, this, 1.f));
	m_TempInputManager->BindAxisDelegate(VK_LEFT, std::bind(&PlayerClass::MoveRight, this, -1.f));
	m_TempInputManager->BindAxisDelegate(VK_UP, std::bind(&PlayerClass::MoveForward, this, -1.f));
	m_TempInputManager->BindAxisDelegate(VK_DOWN, std::bind(&PlayerClass::MoveForward, this, 1.f));
	m_TempInputManager->BindAxisDelegate(VK_SPACE, std::bind(&PlayerClass::FireProjectile, this, 1.f));
}

void PlayerClass::Update(float DeltaTime) {
	Pawn::Update(DeltaTime);

	for (auto Iterator = m_ActivedProjectiles.begin(); Iterator != m_ActivedProjectiles.end();) {
		if (!(*Iterator)->CheckColliding()) {
			(*Iterator)->Update(DeltaTime);
			Iterator++;
		}
	}
}

void PlayerClass::Render(LPD3DXSPRITE Sprite) {
	Pawn::Render(Sprite);
}

void PlayerClass::Destroy() {
	Pawn::Destroy();
}

void PlayerClass::MoveRight(float Value) {
	m_Texture->AddXPosition(Value * m_XMoveSpeed);
}

void PlayerClass::MoveForward(float Value) {
	m_Texture->AddYPosition(Value * m_YMoveSpeed);
}

void PlayerClass::FireProjectile(float Value) {
	auto CurrentTime = std::chrono::system_clock::now();

	if (m_bIsMaximallyActive && m_FireDelay < CurrentTime - m_LastFireTime) {
		ProjectileClass* Projectile = m_Projectiles[m_CurrentProjectileStyle].top();
		if (Projectile) {
			Projectile->SpawnProjectile(m_Texture->GetPosition(), D3DXVECTOR3(1.f, 0.f, 0.f), &m_Projectiles[m_CurrentProjectileStyle], &m_ActivedProjectiles);
			m_Projectiles[m_CurrentProjectileStyle].pop();
			m_ActivedProjectiles.push_back(Projectile);
		}
		m_CurrentActivatedProjectile < m_MaxActivatedProjectile ? m_CurrentActivatedProjectile++ : m_CurrentActivatedProjectile = 0, m_bIsMaximallyActive = true;
		m_LastFireTime = CurrentTime;
	}
}