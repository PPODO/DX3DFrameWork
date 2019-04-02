#include "ProjectileClass.h"
#include "EventClass.h"

ProjectileClass::ProjectileClass() : m_Seta(0.f), m_Target(nullptr), m_bIsExplosion(false), m_ExplosionEffect(nullptr) {
	m_CollisionType = ECT_PROJECTILE;
	EventClass::GetInst()->BindCollisionEvent(this);
	EventClass::GetInst()->BindTriggerEvent(this, [&]() { SetActorIsActivated(false); });
}

ProjectileClass::~ProjectileClass() {
	if (m_ExplosionEffect) {
		delete m_ExplosionEffect;
		m_ExplosionEffect = nullptr;
	}
}

bool ProjectileClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	Actor::Init(Device, FileSrc);

	return true;
}

void ProjectileClass::Update(float DeltaTime, float ActorHeight) {
	if (!m_bIsExplosion) {
		Actor::Update(DeltaTime, ActorHeight);

		if (m_Target && !m_Target->GetActorIsActivated()) {
			m_bIsExplosion = true;
			m_Target = nullptr;
		}

		if (GetActorIsActivated()) {
			ProjectileMovementProcessing(DeltaTime);
		}
	}
}

void ProjectileClass::Render(LPD3DXSPRITE Sprite) {
	if (!m_bIsExplosion) {
		Actor::Render(Sprite);
	}
}

void ProjectileClass::CollisionEventBeginByOtherActor(Actor* OtherActor) {
	if (OtherActor && OtherActor != this && OtherActor->GetActorCollisionType() != m_CollisionType) {
		if (OtherActor->GetActorCollisionType() == ECT_ALLBLOCK) {
			SetActorIsActivated(false);
		}
	}
}

void ProjectileClass::SpawnActor(const D3DXVECTOR3& Location) {
	GetActorImage()->SetPosition(Location);
	SetActorIsActivated(true);
}

void ProjectileClass::ClearObject() {
	
}