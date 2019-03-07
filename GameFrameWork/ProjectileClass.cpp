#include "ProjectileClass.h"
#include "SystemClass.h"

ProjectileClass::ProjectileClass() : m_Owner(nullptr) {
	EventClass::GetInst()->BindTriggerEvent(this, std::bind(&ProjectileClass::OutOfScreen, this));
	m_Collision = CT_PROJECTILE;
	m_bIsActive = false;
}

bool ProjectileClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	Actor::Init(Device, FileSrc, CustomRect);
	
	EventClass::GetInst()->BindCollisionEvent(this);
	return true;
}

void ProjectileClass::Update(float DeltaTime) {
	Actor::Update(DeltaTime);

	ProjectileMoveProcessing();
}

void ProjectileClass::Render(LPD3DXSPRITE Sprite) {
	Actor::Render(Sprite);
}

void ProjectileClass::Destroy() {
	Actor::Destroy();

	if (m_Owner) {
		m_Owner = nullptr;
	}
}

void ProjectileClass::CollisionEventByOtherActor(Actor* OtherActor) {
	if (OtherActor && OtherActor != this && OtherActor != m_Owner && m_Owner->GetActorCollisionType() != OtherActor->GetActorCollisionType()) {
		m_bIsActive = false;
	}
}

void ProjectileClass::SpawnProjectile(Actor* Owner, const D3DXVECTOR3 & Location, const D3DXVECTOR3 & Direction) {
	Owner ? m_Owner = Owner : m_Owner = nullptr;

	m_Texture->SetPosition(Location);
	m_MoveDirection = Direction;
	m_bIsActive = true;
}

void ProjectileClass::ClearObject() {
	m_Owner = nullptr;
	m_bIsActive = false;
}

void ProjectileClass::OutOfScreen() {
	m_bIsActive = false;
}