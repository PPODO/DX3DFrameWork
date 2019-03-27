#include "ProjectileClass.h"
#include "EventClass.h"

ProjectileClass::ProjectileClass() : m_Seta(0.f) {
	m_CollisionType = ECT_PROJECTILE;
	EventClass::GetInst()->BindCollisionEvent(this);
	EventClass::GetInst()->BindTriggerEvent(this, [&]() { SetActorIsActivated(false); });
}

ProjectileClass::~ProjectileClass() {
}

bool ProjectileClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	Actor::Init(Device, FileSrc);

	return true;
}

void ProjectileClass::Update(float DeltaTime, float ActorHeight) {
	Actor::Update(DeltaTime, ActorHeight);

	if (GetActorIsActivated()) {
		ProjectileMovementProcessing(DeltaTime);
	}
}

void ProjectileClass::Render(LPD3DXSPRITE Sprite) {
	Actor::Render(Sprite);
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