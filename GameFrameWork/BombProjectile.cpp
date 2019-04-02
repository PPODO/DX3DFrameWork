#include "BombProjectile.h"
#include "EventClass.h"

BombProjectile::BombProjectile() : m_ExplosionEffect(nullptr) {
	m_ProjectileType = EPT_BOMB;
	m_Name = "Projectile_Bomb";

	m_Height = -10.f;
	m_SaveDeltaTime = 0.f;
	m_PrevLocation = { 0.f,0.f,0.f };
	m_MoveSpeed = { 15.f, 5.f };
	D3DXMatrixTranslation(&m_DefaultMaxtrix, 0, 0, 0);
}

BombProjectile::~BombProjectile() {
}

bool BombProjectile::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	ProjectileClass::Init(Device, L"Projectile/BombProjectile.png");
	
	m_ExplosionEffect = new ParticleClass;
	if (!m_ExplosionEffect || !m_ExplosionEffect->Init(Device, L"Particle/Explosion_Player.png")) {
		return false;
	}
	return true;
}

void BombProjectile::Update(float DeltaTime, float ActorHeight) {
	ProjectileClass::Update(DeltaTime, ActorHeight);

	if (m_bIsExplosion) {
		if (m_ExplosionEffect) {
			m_ExplosionEffect->Update(DeltaTime, ActorHeight);
		}
	}
}

void BombProjectile::Render(LPD3DXSPRITE Sprite) {
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

void BombProjectile::SpawnActor(const D3DXVECTOR3& Location) {
	ProjectileClass::SpawnActor(Location);
	m_Matrix = D3DXMATRIX(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f);
	m_PrevLocation = { 0.f, 0.f, 0.f };
	m_SaveDeltaTime = 0.f;
	m_Height = -10.f;
}

void BombProjectile::CollisionEventBeginByOtherActor(Actor* OtherActor) {
	if (OtherActor && OtherActor != this) {
		if (OtherActor->GetActorCollisionType() == ECT_PLAYER) {

		}
		else if (OtherActor->GetActorCollisionType() == ECT_PROJECTILE) {
			ProjectileClass* Projectile = (ProjectileClass*)OtherActor;
			if (Projectile && Projectile->GetProjectileType() != m_ProjectileType) {
				if (m_ExplosionEffect) {
					m_ExplosionEffect->SpawnActor(GetActorImage()->GetPosition());
					m_ExplosionEffect->BindEndNotification([&]() { SetActorIsActivated(false); });
					m_bIsExplosion = true;
				}
			}
		}
		else if (OtherActor->GetActorCollisionType() == ECT_ALLBLOCK) {
			if (m_ExplosionEffect) {
				float Height = OtherActor->GetActorImage()->GetPosition().y - GetActorImage()->GetPosition().y;
				m_ExplosionEffect->SpawnActor(D3DXVECTOR3(GetActorImage()->GetPosition().x, Height + (GetActorImage()->GetPosition().y - m_ExplosionEffect->GetActorImage()->GetImageCenter().y), 0.f));
				m_ExplosionEffect->BindEndNotification([&]() { SetActorIsActivated(false); });
				m_bIsExplosion = true;
			}
		}
	}
}

void BombProjectile::ClearObject() {
	m_bIsExplosion = false;
}

void BombProjectile::ProjectileMovementProcessing(const float & DeltaTime) {
	if (!m_bIsExplosion) {
		D3DXVECTOR3 NewLocation(0.f, 0.f, 0.f);
		m_PrevLocation = GetActorImage()->GetPosition();

		m_Height += Gravity;
		m_SaveDeltaTime += DeltaTime;
		NewLocation.x = m_MoveSpeed.x * cos(D3DXToRadian(90 - m_Seta)) * m_SaveDeltaTime;
		NewLocation.y = -(m_MoveSpeed.y * sin(D3DXToRadian(90 + m_Seta)) - m_Height * (pow(m_SaveDeltaTime, 2) / 2));
		GetActorImage()->AddPosition(NewLocation);
		D3DXVECTOR3 Distance = GetActorImage()->GetPosition() - m_PrevLocation;

		D3DXMatrixTranslation(&m_CenterLocation, -GetActorImage()->GetPosition().x, -GetActorImage()->GetPosition().y, 0.f);
		D3DXMatrixRotationZ(&m_Rotation, D3DXToRadian(90 - m_Seta) + D3DXToRadian(m_Seta) - (atan2(Distance.x, Distance.y)));
		D3DXMatrixTranslation(&m_Location, GetActorImage()->GetPosition().x, GetActorImage()->GetPosition().y, 0.f);
		m_Matrix = m_CenterLocation * m_Rotation * m_Location;
	}
}