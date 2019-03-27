#include "EnemyClass.h"
#include "EventClass.h"
#include "ParticleClass.h"
#include "ProjectileClass.h"
#include "ObjectPoolClass.h"

EnemyClass::EnemyClass() : m_PoolManager(nullptr) {
	m_ProjectileType = EPT_DEFAULT;
	m_MoveSpeed = { 5.f, 10.f };

	m_MaxActivatedProjectile = 10;
	m_ProjectileObjects.resize(EPT_COUNT);
	EventClass::GetInst()->BindTriggerEvent(this, [this]() { SetActorIsActivated(false); });
}

EnemyClass::~EnemyClass() {
}

bool EnemyClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	PawnClass::Init(Device, FileSrc);

	m_ExplosionEffect;
	if (m_ExplosionEffect) {

	}

	return true;
}

void EnemyClass::Update(float DeltaTime, float ActorHeight) {
	PawnClass::Update(DeltaTime, ActorHeight);

	if (CanFire()) {
		auto Projectile = FireProjectile(D3DXVECTOR3(-(GetActorImage()->GetImageCenter().x / 2), GetActorImage()->GetImageCenter().y, 0.f));
		if (Projectile) {
			Projectile->SetMoveDirection(D3DXVECTOR3(0, 1.f, 0.f));
			Projectile->SetProjectileSeta(0.f);
		}
	}
	EnemyMovementProcessing();
}

void EnemyClass::Render(LPD3DXSPRITE Sprite) {
	PawnClass::Render(Sprite);

}

bool EnemyClass::IsItOutOfScreen() {
	if (GetActorImage()->GetPosition().x + GetActorImage()->GetImageCenter().x < 0 || GetActorImage()->GetPosition().x - GetActorImage()->GetImageCenter().x > GetWindowSize().right) {
		return true;
	}
	return false;
}

void EnemyClass::CollisionEventBeginByOtherActor(Actor* OtherActor) {
	if (OtherActor && OtherActor != this && OtherActor->GetActorCollisionType() != ECT_ENEMY) {

	}
}

void EnemyClass::SpawnActor(const D3DXVECTOR3& Location) {
	D3DXVECTOR3 SpawnLocation(FLOAT(GetWindowSize().right) - GetActorImage()->GetImageCenter().x, GetActorImage()->GetImageCenter().y * 2, 0.f);

	if (m_PoolManager) {
		m_PoolManager->GetPoolObject("Projectile_Bomb", m_ProjectileObjects[m_ProjectileType], 10);
	}
	GetActorImage()->SetPosition(Location);
	SetActorIsActivated(true);
}

void EnemyClass::ClearObject() {
	if (m_PoolManager) {
		for (auto& It : m_ActivatedProjectileObjects) {
			if (It) {
				m_ProjectileObjects[It->GetProjectileType()].push(It);
			}
		}
		m_ActivatedProjectileObjects.clear();

		for (auto& It : m_ProjectileObjects) {
			if (It.size() > 0) {
				m_PoolManager->ReleaseAll(It.top()->GetName(), It, It.size());
			}
		}
	}
	m_PoolManager = nullptr;
}