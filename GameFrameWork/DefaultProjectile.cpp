#include "DefaultProjectile.h"
#include "EventClass.h"

DefaultProjectile::DefaultProjectile() {
	m_ProjectileType = EPT_DEFAULT;
	m_Name = "Projectile_Default";

	m_MoveSpeed = { 15.f, 10.f };
	m_SaveDeltaTime = m_Height = 0;
	D3DXMatrixTranslation(&m_DefaultMaxtrix, 0, 0, 0);
}

DefaultProjectile::~DefaultProjectile() {
}

bool DefaultProjectile::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	ProjectileClass::Init(Device, L"Projectile/DefaultProjectile.png");

	return true;
}

void DefaultProjectile::Update(float DeltaTime) {
	ProjectileClass::Update(DeltaTime);
	
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
	m_Height = m_SaveDeltaTime = 0.f;
}

void DefaultProjectile::ProjectileMovementProcessing(const float& DeltaTime) {
	D3DXVECTOR3 NewPosition = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 Distance;

	m_Height += Gravity;
	m_SaveDeltaTime += DeltaTime;

	NewPosition.x += m_MoveSpeed.x * cos(D3DXToRadian(90 - m_Seta)) * m_SaveDeltaTime * 2;
	NewPosition.y -= m_MoveSpeed.y * sin(D3DXToRadian(90 - m_Seta)) - m_Height * pow(m_SaveDeltaTime, 2) / 2;

	m_PrevLocation = GetActorImage()->GetPosition();
	GetActorImage()->AddPosition(NewPosition);
	Distance = GetActorImage()->GetPosition() - m_PrevLocation;

	D3DXMatrixTranslation(&m_CenterLocation, -GetActorImage()->GetPosition().x, -GetActorImage()->GetPosition().y, 0);
	D3DXMatrixRotationZ(&m_Rotation, (-1 * atan2(Distance.x, Distance.y) - D3DXToRadian(90 - (m_Seta * 0.9))));
	D3DXMatrixTranslation(&m_Location, GetActorImage()->GetPosition().x, GetActorImage()->GetPosition().y, 0);
	m_Matrix = m_CenterLocation * m_Rotation * m_Location;
}