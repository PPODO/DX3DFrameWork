#include "NuclearProjectileClass.h"
#include "EventClass.h"

NuclearProjectileClass::NuclearProjectileClass() {
	m_bIsActivated = false;
	m_ProjectileType = EPT_NUCLEAR;
	m_Name = "Projectile_Nuclear";

	m_Height = -10.f;
	m_SaveDeltaTime = 0.f;
	m_PrevLocation = { 0.f,0.f,0.f };
	m_MoveSpeed = { 15.f, 5.f };
	D3DXMatrixTranslation(&m_DefaultMaxtrix, 0, 0, 0);
}

NuclearProjectileClass::~NuclearProjectileClass() {
}

bool NuclearProjectileClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	ProjectileClass::Init(Device, L"Projectile/NuclearProjectile.png");

	m_ExplosionEffect = new ParticleClass;
	if (!m_ExplosionEffect || !m_ExplosionEffect->Init(Device, L"Particle/Nuclear_Explosion.png")) {
		return false;
	}
	m_ExplosionEffect->SetActorCollisionType(ECT_ALLBLOCK);
	EventClass::GetInst()->BindCollisionEvent(m_ExplosionEffect);

	return true;
}

void NuclearProjectileClass::Update(float DeltaTime, float ActorHeight) {
	ProjectileClass::Update(DeltaTime, ActorHeight);

	if (m_bIsExplosion) {
		if (m_ExplosionEffect) {
			m_ExplosionEffect->Update(DeltaTime, ActorHeight);
		}
	}
}

void NuclearProjectileClass::Render(LPD3DXSPRITE Sprite) {
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

void NuclearProjectileClass::CollisionEventBeginByOtherActor(Actor* OtherActor) {
	if (OtherActor && OtherActor != this && OtherActor->GetActorCollisionType() != m_CollisionType) {
		if (OtherActor->GetActorCollisionType() != ECT_PLAYER && OtherActor != GetOwner()) {
			if (m_ExplosionEffect) {
				m_ExplosionEffect->SpawnActor(m_ExplosionEffect->GetActorImage()->GetImageCenter());
				m_ExplosionEffect->BindEndNotification([&]() { SetActorIsActivated(false); m_bIsExplosion = false; });
				m_bIsExplosion = true;
			}
		}
	}
}

void NuclearProjectileClass::SpawnActor(const D3DXVECTOR3& Location) {
	ProjectileClass::SpawnActor(Location);
	m_Matrix = D3DXMATRIX(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f);
	m_PrevLocation = { 0.f, 0.f, 0.f };
	m_SaveDeltaTime = 0.f;
	m_Height = -10.f;
}

void NuclearProjectileClass::ProjectileMovementProcessing(const float& DeltaTime) {
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