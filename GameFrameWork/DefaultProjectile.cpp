#include "DefaultProjectile.h"
#include "EventClass.h"

DefaultProjectile::DefaultProjectile() {
	m_ProjectileType = EPT_DEFAULT;
	m_Name = "Projectile_Default";

	m_MoveSpeed = { 20.f, 0.f };
	D3DXMatrixTranslation(&m_DefaultMaxtrix, 0, 0, 0);
	EventClass::GetInst()->BindCollisionEvent(this);
}

DefaultProjectile::~DefaultProjectile() {
}

bool DefaultProjectile::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	ProjectileClass::Init(Device, L"Projectile/DefaultProjectile.png");

	return true;
}

void DefaultProjectile::Update(float DeltaTime, float ActorHeight) {
	ProjectileClass::Update(DeltaTime, ActorHeight);
	
}

void DefaultProjectile::Render(LPD3DXSPRITE Sprite) {
	if (GetActorIsActivated()) {
		Sprite->SetTransform(&m_Matrix);
		ProjectileClass::Render(Sprite);
		Sprite->SetTransform(&m_DefaultMaxtrix);
	}
}

void DefaultProjectile::CollisionEventBeginByOtherActor(Actor* OtherActor) {
	if (OtherActor && OtherActor != this && OtherActor->GetActorCollisionType() != m_CollisionType) {
		if (OtherActor->GetActorCollisionType() == ECT_ALLBLOCK) {
			SetActorIsActivated(false);
		}
	}
}

void DefaultProjectile::SpawnActor(const D3DXVECTOR3& Location) {
	ProjectileClass::SpawnActor(Location);
	m_Matrix = D3DXMATRIX(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f);
}

void DefaultProjectile::ProjectileMovementProcessing(const float& DeltaTime) {
	GetActorImage()->AddPosition(D3DXVECTOR3(m_MoveDirection.x, m_MoveDirection.y, 0.f) * m_MoveSpeed.x);

	D3DXMatrixTranslation(&m_CenterLocation, -GetActorImage()->GetPosition().x, -GetActorImage()->GetPosition().y, 0.f);
	D3DXMatrixRotationZ(&m_Rotation, D3DXToRadian(90 + m_Seta));
	D3DXMatrixTranslation(&m_Location, GetActorImage()->GetPosition().x, GetActorImage()->GetPosition().y, 0.f);
	m_Matrix = m_CenterLocation * m_Rotation * m_Location;
}