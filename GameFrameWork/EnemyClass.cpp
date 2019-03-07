#include "EnemyClass.h"

std::random_device EnemyClass::m_RandomDevice;
std::mt19937_64 EnemyClass::m_RandomAlgorithm;
std::uniform_real_distribution<float> EnemyClass::m_Random;

EnemyClass::EnemyClass() : m_MinSpawnDelay(0.f), m_MaxSpawnDelay(0.f), m_bIsSpawn(false) {
	m_Projectiles.resize(PS_DEFAULT + 1);

	m_Collision = CT_ENEMY;
	m_bIsActive = false;
	m_MoveDirection = D3DXVECTOR3(-1.f, 0.f, 0.f);
	m_FireDelay = std::chrono::duration<float>(0.5);
	m_MaxActiveProjectile = 5;
	m_RandomAlgorithm = std::mt19937_64(m_RandomDevice());
}

EnemyClass::~EnemyClass() {
}

bool EnemyClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	Pawn::Init(Device, FileSrc, CustomRect);

	return true;
}

void EnemyClass::Update(float DeltaTime) {
	Pawn::Update(DeltaTime);

	if (!m_bIsSpawn && m_SpawnDelay < std::chrono::system_clock::now() - m_LastSpawnTime) {
		m_bIsSpawn = true;
	}

	if (m_bIsActive && m_bIsSpawn) {
		SpawnProjectile(m_MoveDirection);
		ProcessingEnemyMovement();
	}
}

void EnemyClass::Render(LPD3DXSPRITE Sprite) {
	Pawn::Render(Sprite);
}

void EnemyClass::Destroy() {
	Pawn::Destroy();
}

void EnemyClass::CollisionEventByOtherActor(Actor* OtherActor) {
	if (OtherActor && OtherActor != this && OtherActor->GetActorCollisionType() != m_Collision) {
		if (OtherActor->GetActorCollisionType() == CT_PLAYER) {
			m_bIsActive = false;
		}
		else if (OtherActor->GetActorCollisionType() == CT_PROJECTILE) {
			ProjectileClass* Projectile = (ProjectileClass*)OtherActor;
			if (Projectile && Projectile->GetOwner()->GetActorCollisionType() != m_Collision) {
				m_bIsActive = false;
			}
		}
	}
}

void EnemyClass::PoolThisObject(std::stack<EnemyClass*>& EnemyList, std::vector<EnemyClass*>& ActivatedEnemy, std::vector<EnemyClass*>::iterator& Iterator) {
	EnemyList.push((*Iterator));
	Iterator = ActivatedEnemy.erase(Iterator);
	ClearObject();
}

void EnemyClass::ClearObject() {
	ClearProjectilePool();
	m_bIsActive = false;
}

void EnemyClass::SpawnObject() {
	m_Projectiles.resize(PS_DEFAULT + 1);

	m_Random = std::uniform_real_distribution<float>(GetWindowSize().top + m_Texture->GetImageCenter().y * 2, GetWindowSize().bottom - m_Texture->GetImageCenter().y * 2);
	m_Texture->SetPosition(D3DXVECTOR3(FLOAT(GetWindowSize().right) + m_Texture->GetImageCenter().x, m_Random(m_RandomAlgorithm), 0.f));

	m_Random = std::uniform_real_distribution<float>(m_MinSpawnDelay, m_MaxSpawnDelay);
	m_SpawnDelay = std::chrono::duration<float>(m_Random(m_RandomAlgorithm));

	m_PoolManager->GetPoolObject("DefaultProjectile", m_Projectiles[PS_DEFAULT], m_MaxActiveProjectile);
	m_LastSpawnTime = std::chrono::system_clock::now();
	m_bIsActive = true;
}

void EnemyClass::OutOfScreen() {
	m_bIsActive = false;
}

void EnemyClass::ProcessingEnemyMovement() {
	m_Texture->AddPosition(m_MoveDirection * ((m_XMoveSpeed + m_YMoveSpeed) / 2));
}