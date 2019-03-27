#include "PawnClass.h"
#include "SystemClass.h"
#include "ParticleClass.h"
#include "ObjectPoolClass.h"

RECT PawnClass::m_WindowSize = { 0,0,0,0 };

PawnClass::PawnClass() : m_bStartMoveToLocation(true), m_bUseGravity(true), m_bLanded(true), m_bIsJumping(false), m_SaveYLocation(0.f), m_CurrentHeight(0.f), m_MaxHeight(-12.5f), m_LocationIsHaveToGo(0.f) {
	m_WindowSize = SystemClass::GetInst()->GetWindowSize();
	m_bIsActivated = true;
	m_bIsFire = false;
	m_ProjectileType = EPT_DEFAULT;
	m_PoolManager = nullptr;
	m_ExplosionEffect = nullptr;
	m_CopyImage = nullptr;

	m_ProjectileObjects.resize(EPT_COUNT);
}

PawnClass::~PawnClass() {
	if (m_CopyImage) {
		delete m_CopyImage;
		m_CopyImage = nullptr;
	}

	for (auto& It : m_ActivatedProjectileObjects) {
		if (It) {
			m_ProjectileObjects[It->GetProjectileType()].push(It);
		}
	}
	m_ActivatedProjectileObjects.clear();

	for (auto& It : m_ProjectileObjects) {
		if (GetPoolManager() && It.size() > 0) {
			GetPoolManager()->ReleaseAll(It.top()->GetName(), It, It.size());
		}
	}
}

bool PawnClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	Actor::Init(Device, FileSrc);

	m_CopyImage = new TextureClass(Device, FileSrc);
	if (!m_CopyImage) {
		return false;
	}

	SetupPlayerInput();
	return true;
}

void PawnClass::Update(float DeltaTime, float ActorHeight) {
	Actor::Update(DeltaTime, ActorHeight);

	if (m_bStartMoveToLocation) {
		PlayStartMoveToLocation();
	}
	CalculateJump();
	CalculateProjectile(DeltaTime, ActorHeight);

	if (m_ExplosionEffect) {
		if (m_CopyImage && m_ExplosionEffect->GetActorIsActivated()) {
			m_CopyImage->AddYPosition(ActorHeight);
		}
		m_ExplosionEffect->GetActorImage()->AddYPosition(ActorHeight);
		m_ExplosionEffect->Update(DeltaTime, ActorHeight);
	}
}

void PawnClass::Render(LPD3DXSPRITE Sprite) {
	Actor::Render(Sprite);

	for (const auto It : m_ActivatedProjectileObjects) {
		if (It && It->GetActorIsActivated()) {
			It->Render(Sprite);
		}
	}

	if (m_ExplosionEffect) {
		if (m_CopyImage && m_ExplosionEffect->GetActorIsActivated()) {
			m_CopyImage->Render(Sprite);
		}
		m_ExplosionEffect->Render(Sprite);
	}
}

ProjectileClass* PawnClass::FireProjectile(const D3DXVECTOR3& Offset) {
	if (m_ActivatedProjectileObjects.size() < m_MaxActivatedProjectile) {
		if (m_ProjectileObjects[m_ProjectileType].size() > 0) {
			auto Projectile = m_ProjectileObjects[m_ProjectileType].top();
			if (Projectile) {
				Projectile->SpawnActor(GetActorImage()->GetPosition() + Offset);
				m_ActivatedProjectileObjects.push_back(Projectile);
				m_ProjectileObjects[m_ProjectileType].pop();
			}
			return Projectile;
		}
	}
	return nullptr;
}

void PawnClass::CalculateProjectile(const float& DeltaTime, float ActorHeight) {
	for (auto Iterator = m_ActivatedProjectileObjects.begin(); Iterator != m_ActivatedProjectileObjects.end();) {
		if (Iterator != m_ActivatedProjectileObjects.cend() && (*Iterator)->GetActorIsActivated()) {
			(*Iterator)->Update(DeltaTime, ActorHeight);
			++Iterator;
		}
		else {
			(*Iterator)->ClearObject();
			m_ProjectileObjects[(*Iterator)->GetProjectileType()].push((*Iterator));
			Iterator = m_ActivatedProjectileObjects.erase(Iterator);
		}
	}
}

void PawnClass::SpawnExplosionEffect() {
	if (m_ExplosionEffect) {
		m_CopyImage->SetPosition(GetActorImage()->GetPosition());
		m_ExplosionEffect->SpawnActor(D3DXVECTOR3(GetActorImage()->GetPosition().x, GetActorImage()->GetPosition().y - (m_ExplosionEffect->GetActorImage()->GetImageCenter().y / 3), 0.f));
	}
}