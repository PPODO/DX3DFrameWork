#include "EnemyClass.h"
#include "EventClass.h"
#include "ProjectileClass.h"

EnemyClass::EnemyClass() {
	m_ProjectileType = EPT_DEFAULT;
	m_MoveSpeed = { 5.f, 0.f };

	EventClass::GetInst()->BindTriggerEvent(this, [this]() { SetActorIsActivated(false); });
}

EnemyClass::~EnemyClass() {
}

bool EnemyClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	PawnClass::Init(Device, FileSrc);

	return true;
}

void EnemyClass::Update(float DeltaTime) {
	PawnClass::Update(DeltaTime);

	EnemyMovementProcessing();
}

void EnemyClass::Render(LPD3DXSPRITE Sprite) {
	PawnClass::Render(Sprite);

}

void EnemyClass::CollisionEventBeginByOtherActor(Actor* OtherActor) {
	if (OtherActor && OtherActor != this && OtherActor->GetActorCollisionType() != ECT_ENEMY) {

	}
}

void EnemyClass::SpawnActor(const D3DXVECTOR3& Location) {
	GetActorImage()->SetPosition(Location);
	SetActorIsActivated(true);
}

ProjectileClass * EnemyClass::FireProjectile(const D3DXVECTOR3 & Offset) {

	return nullptr;
}

void EnemyClass::CalculateProjectile(const float & DeltaTime) {

}

void EnemyClass::SetProjectileObject(std::vector<std::stack<class ProjectileClass*>>& List) {
	m_ProjectileObjects = &List;
}

void EnemyClass::ReleaseProjectileObject() {
	if (m_ProjectileObjects) {
		for (auto& It : m_ActivatedProjectile) {
			(*m_ProjectileObjects)[It.second].push(It.first);
		}
		m_ActivatedProjectile.clear();
	}
	m_ProjectileObjects = nullptr;
}