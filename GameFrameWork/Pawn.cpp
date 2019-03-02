#include "Pawn.h"
#include "SystemClass.h"
#include "ActorClass.h"
#include "ProjectileClass.h"

ObjectPoolClass* Pawn::m_PoolManager = nullptr;

Pawn::Pawn() : m_XMoveSpeed(0.f), m_YMoveSpeed(0.f), m_FireDelay(0.f), m_MaxActivatedProjectile(0), m_CurrentProjectileStyle(PS_DEFAULT) {
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
	ClearProjectilePool();
	if (m_Texture) {
		delete m_Texture;
		m_Texture = nullptr;
	}
}

void Pawn::SpawnProjectile(D3DXVECTOR3&& Direction) {
	auto CurrentTime = std::chrono::system_clock::now();

	if (m_ActivedProjectiles.size() < m_MaxActivatedProjectile && m_FireDelay < CurrentTime - m_LastFireTime) {
		ProjectileClass* Projectile = m_Projectiles[m_CurrentProjectileStyle].top();
		if (Projectile) {
			Projectile->SpawnProjectile(m_Texture->GetPosition(), Direction, &m_Projectiles[m_CurrentProjectileStyle], &m_ActivedProjectiles);
			m_Projectiles[m_CurrentProjectileStyle].pop();
			m_ActivedProjectiles.push_back(Projectile);
		}
		m_LastFireTime = CurrentTime;
	}
}

bool Pawn::IsCrashed(const Actor* Object) {
	if (Object) {
		for (auto Iterator = m_ActivedProjectiles.begin(); Iterator != m_ActivedProjectiles.end(); ++Iterator) {
			if ((*Iterator)->IsCrash(Object)) {
				(*Iterator)->PoolThisObject(Iterator);
				return true;
			}
		}
	}
	return false;
}

void Pawn::ClearProjectilePool() {
	for (auto It : m_Projectiles) {
		for (auto Iterator = m_ActivedProjectiles.begin(); Iterator != m_ActivedProjectiles.end();) {
			if (*Iterator) {
				It.push(*Iterator);
				Iterator = m_ActivedProjectiles.erase(Iterator);
			}
		}
		if (It.size() > 0) {
			if (!m_PoolManager) {
				m_PoolManager = SystemClass::GetInst()->GetActorManager()->GetPoolManager();
			}
			m_PoolManager->ReleaseAll(It.top()->GetName(), It, It.size());
		}
	}
}