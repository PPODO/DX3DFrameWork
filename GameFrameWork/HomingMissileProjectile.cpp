#include "HomingMissileProjectile.h"
#include "EventClass.h"

HomingMissileProjectile::HomingMissileProjectile() {
	m_ProjectileType = EPT_HOMINGMISSILE;
	m_Name = "Projectile_Homing";

	D3DXMatrixTranslation(&m_DefaultMaxtrix, 0, 0, 0);
}

HomingMissileProjectile::~HomingMissileProjectile() {
}

bool HomingMissileProjectile::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	ProjectileClass::Init(Device, L"Projectile/HomingProjectile.png");

	m_ExplosionEffect = new ParticleClass;
	if (!m_ExplosionEffect || !m_ExplosionEffect->Init(Device, L"Particle/Explosion_Player.png")) {
		return false;
	}
	m_ExplosionEffect->BindEndNotification([this]() { SetActorIsActivated(false); });
	return true;
}

void HomingMissileProjectile::Update(float DeltaTime, float ActorHeight) {
	ProjectileClass::Update(DeltaTime, ActorHeight);

	if (m_bIsExplosion && !m_ExplosionEffect->GetActorIsActivated()) {
		m_ExplosionEffect->SpawnActor(D3DXVECTOR3(GetActorImage()->GetPosition().x, GetActorImage()->GetPosition().y + ActorHeight, 0.f));
	}
	if (m_ExplosionEffect->GetActorIsActivated()) {
		m_ExplosionEffect->Update(DeltaTime, ActorHeight);
	}
}

void HomingMissileProjectile::Render(LPD3DXSPRITE Sprite) {
	if (GetActorIsActivated()) {
		Sprite->SetTransform(&m_Matrix);
		ProjectileClass::Render(Sprite);
		Sprite->SetTransform(&m_DefaultMaxtrix);
	}
	if (m_bIsExplosion) {
		if (m_ExplosionEffect) {
			m_ExplosionEffect->Render(Sprite);
		}
	}
}

void HomingMissileProjectile::CollisionEventBeginByOtherActor(Actor* OtherActor) {
	if (OtherActor && OtherActor != this && OtherActor->GetActorCollisionType() != m_CollisionType) {
		if (OtherActor->GetActorCollisionType() == ECT_ENEMY) {
			m_bIsExplosion = true;
		}
	}
}

void HomingMissileProjectile::SpawnActor(const D3DXVECTOR3& Location) {
	ProjectileClass::SpawnActor(Location);
	m_bIsExplosion = false;
}

void HomingMissileProjectile::ProjectileMovementProcessing(const float& DeltaTime) {
	float ZRotation = 0.f;
	if (m_Target) {
		D3DXVECTOR3 CrossResult;
		D3DXVECTOR3 Direction = m_Target->GetActorImage()->GetPosition() - GetActorImage()->GetPosition();
		D3DXVec3Normalize(&Direction, &Direction);
		ZRotation = D3DXVec3Cross(&CrossResult, &Direction, &(D3DXVECTOR3(0.f, 1.f, 1.f)))->z;

		GetActorImage()->AddPosition(Direction * 6.f);
	}

	D3DXMatrixTranslation(&m_CenterLocation, -GetActorImage()->GetPosition().x, -GetActorImage()->GetPosition().y, 0.f);
	D3DXMatrixRotationZ(&m_Rotation, ZRotation + D3DXToRadian(90));
	D3DXMatrixTranslation(&m_Location, GetActorImage()->GetPosition().x, GetActorImage()->GetPosition().y, 0.f);
	m_Matrix = m_CenterLocation * m_Rotation * m_Location;
}