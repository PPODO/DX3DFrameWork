#include "Pawn.h"
#include "SystemClass.h"
#include "ActorClass.h"
#include "ProjectileClass.h"

ObjectPoolClass* Pawn::m_PoolManager = nullptr;

Pawn::Pawn() : m_XMoveSpeed(0.f), m_YMoveSpeed(0.f), m_FireDelay(0.f), m_MaxActivatedProjectile(0), m_CurrentProjectileStyle(PS_DEFAULT), m_Health(0.f), m_bIsDead(false), m_UseAutoSpawn(false) {
	SystemClass::GetInst()->GetEventClass()->BindTriggerEvent(this, std::bind(&Pawn::OutOfScreen, this));
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

	if (m_UseAutoSpawn) {
		SpawnProjectile(m_ProjectileDirection);
	}
	for (auto Iterator = m_ActivedProjectiles.begin(); Iterator != m_ActivedProjectiles.end();) {
		if ((*Iterator)->GetIsActivation()) {
			(*Iterator)->Update(DeltaTime);
			Iterator++;
		}
		else {
			(*Iterator)->PoolThisObject(Iterator);
		}
	}
}

void Pawn::Render(LPD3DXSPRITE Sprite) {
	if (!m_bIsDead) {
		Actor::Render(Sprite);
	}

	for (auto Iterator : m_ActivedProjectiles) {
		if (Iterator->GetIsActivation()) {
			Iterator->Render(Sprite);
		}
	}
}

void Pawn::Destroy() {
	Actor::Destroy();

	ClearProjectilePool();
}

void Pawn::SetProjectiles(std::stack<class ProjectileClass*>& List, size_t MaxListProjectile, float FireDelay, bool UseAuto, const D3DXVECTOR3& Direction) {
	m_UseAutoSpawn = UseAuto;
	m_ProjectileDirection = Direction;
	m_Projectiles.push_back(List);
	m_MaxActivatedProjectile = MaxListProjectile;
	m_FireDelay = std::chrono::duration<float>(FireDelay);
}

void Pawn::SpawnProjectile(const D3DXVECTOR3& Direction) {
	auto CurrentTime = std::chrono::system_clock::now();

	if (m_ActivedProjectiles.size() < m_MaxActivatedProjectile && m_FireDelay < CurrentTime - m_LastFireTime) {
		ProjectileClass* Projectile = m_Projectiles[m_CurrentProjectileStyle].top();
		if (Projectile) {
			D3DXVECTOR3 Temp(m_Texture->GetImageCenter().x * Direction.x, m_Texture->GetImageCenter().y * Direction.y, 0.f);
			Temp += Projectile->GetTexture()->GetImageCenter();
			Projectile->SpawnProjectile(this, m_Texture->GetPosition() + Temp, Direction, &m_Projectiles[m_CurrentProjectileStyle], &m_ActivedProjectiles);
			m_Projectiles[m_CurrentProjectileStyle].pop();
			m_ActivedProjectiles.push_back(Projectile);
		}
		m_LastFireTime = CurrentTime;
	}
}

void Pawn::ClearActivatedProjectile() {
	for (auto& It : m_Projectiles) {
		for (auto Iterator = m_ActivedProjectiles.begin(); Iterator != m_ActivedProjectiles.end();) {
			if (Iterator != m_ActivedProjectiles.cend()) {
				It.push(*Iterator);
				Iterator = m_ActivedProjectiles.erase(Iterator);
			}
		}
	}
}

void Pawn::ClearProjectilePool() {
	for (auto& It : m_Projectiles) {
		for (auto Iterator = m_ActivedProjectiles.begin(); Iterator != m_ActivedProjectiles.end();) {
			if (Iterator != m_ActivedProjectiles.cend()) {
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