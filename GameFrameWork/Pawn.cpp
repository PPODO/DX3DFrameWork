#include "Pawn.h"
#include "SystemClass.h"
#include "ActorClass.h"
#include "ProjectileClass.h"
#include "EventClass.h"

ObjectPoolClass* Pawn::m_PoolManager = nullptr;

Pawn::Pawn() : m_XMoveSpeed(0.f), m_YMoveSpeed(0.f), m_MaxActiveProjectile(0), m_CurrentVelocityY(0.f), m_bIsJumping(false), m_Landed(false), m_bIsFalling(false), m_CurrentProjectileStyle(PS_DEFAULT) {
	EventClass::GetInst()->BindTriggerEvent(this, std::bind(&Pawn::OutOfScreen, this));
	EventClass::GetInst()->BindCollisionEvent(this);
	m_LastFireTime = std::chrono::system_clock::now();
}

Pawn::~Pawn() {
}

bool Pawn::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	Actor::Init(Device, FileSrc, CustomRect);
	
	if (!m_PoolManager) {
		return false;
	}

	SetupPlayerInput();
	return true;
}

void Pawn::SetupPlayerInput() {
}

void Pawn::Update(float DeltaTime) {
	Actor::Update(DeltaTime);

	if (!m_Landed && !m_bIsJumping) {
		m_Texture->AddYPosition(5.f);
		m_bIsFalling = true;
	}

	if (m_bIsJumping && !m_Landed) {
		if (m_CurrentVelocityY <= 0.f) {
			m_bIsFalling = true;
		}
		m_Texture->AddYPosition(m_CurrentVelocityY);
		m_CurrentVelocityY += Gravity;
	}

	for (auto It = m_ActivatedProjectile.begin(); It != m_ActivatedProjectile.end();) {
		if ((*It) && (*It)->GetIsActivation()) {
			(*It)->Update(DeltaTime);
			++It;
		}
		else {
			(*It)->PoolThisObject(m_Projectiles[m_PoolManager->GetKeyByObjectName((*It)->GetName())], m_ActivatedProjectile, It);
		}
	}
}

void Pawn::Render(LPD3DXSPRITE Sprite) {
	Actor::Render(Sprite);

	for (const auto& It : m_ActivatedProjectile) {
		if (It) {
			It->Render(Sprite);
		}
	}
}

void Pawn::Destroy() {
	Actor::Destroy();

	ClearProjectilePool();
}

void Pawn::ClearProjectile() {
	for (auto& It : m_ActivatedProjectile) {
		It->SetActivation(false);
		m_Projectiles[m_PoolManager->GetKeyByObjectName(It->GetName())].push(It);
	}
	m_ActivatedProjectile.clear();
}

void Pawn::ClearProjectilePool() {
	ClearProjectile();

	for (auto It : m_Projectiles) {
		if (It.size() > 0) {
			m_PoolManager->ReleaseAll(It.top()->GetName(), It, It.size());
		}
	}
	m_Projectiles.clear();
}

void Pawn::SpawnProjectile(const D3DXVECTOR3& Direction) {
	auto CurrentTime = std::chrono::system_clock::now();

	if (m_ActivatedProjectile.size() < m_MaxActiveProjectile && m_Projectiles[m_CurrentProjectileStyle].size() > 0 && m_FireDelay < CurrentTime - m_LastFireTime) {
		ProjectileClass* Projectile = m_Projectiles[m_CurrentProjectileStyle].top();
		if (Projectile) {
			Projectile->SpawnProjectile(this, D3DXVECTOR3(m_Texture->GetPosition().x + (Direction.x * m_Texture->GetImageCenter().x), m_Texture->GetPosition().y, 0.f), Direction);
			m_ActivatedProjectile.push_back(Projectile);
			m_Projectiles[m_CurrentProjectileStyle].pop();
		}
		m_LastFireTime = CurrentTime;
	}
}