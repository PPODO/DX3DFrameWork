#include "EnemyClass.h"
#include "SystemClass.h"
#include "ActorClass.h"
#include "EventClass.h"
#include "DefaultProjectileClass.h"

std::random_device EnemyClass::m_RandomDevice;
std::mt19937_64 EnemyClass::m_RandomAlgorithm;
std::uniform_real_distribution<float> EnemyClass::m_Random;

std::vector<std::stack<EnemyClass*>>* EnemyClass::m_ObjectList = nullptr;
std::vector<EnemyClass*>* EnemyClass::m_ActivatedList = nullptr;

TextureClass* EnemyClass::m_Target = nullptr;

EnemyClass::EnemyClass() {
	m_Projectiles.resize(PS_DEFAULT + 1);

	m_Health = 9.f;
	m_Collision = CT_ENEMY;
	m_MaxActivatedProjectile = 5;
	m_RandomAlgorithm = std::mt19937_64(m_RandomDevice());
	m_FireDelay = std::chrono::duration<float>(0.25f);
	m_bIsActive = false;
	m_UseAutoSpawn = true;
	SystemClass::GetInst()->GetEventClass()->BindCollisionEvent(this);
}

EnemyClass::~EnemyClass() {
	if (m_Target) {
		m_Target = nullptr;
	}
	if (m_ActivatedList) {
		m_ActivatedList = nullptr;
	}
	if (m_ObjectList) {
		m_ObjectList = nullptr;
	}
}

bool EnemyClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	Pawn::Init(Device, FileSrc, CustomRect);
	if (!GetPoolManager()) {
		SetPoolManager(SystemClass::GetInst()->GetActorManager()->GetPoolManager());
	}
	return true;
}

void EnemyClass::Update(float DeltaTime) {
	if (!m_bIsActive && m_SpawnTime < std::chrono::system_clock::now() - m_LastSpawnTime) {
		m_bIsActive = true;
	}

	if (m_bIsActive) {
		if (!m_bIsDead && m_UseAutoSpawn) {
			SpawnProjectile(m_ProjectileDirection);
		}
		for (auto Iterator = m_ActivedProjectiles.begin(); Iterator != m_ActivedProjectiles.end();) {
			if ((*Iterator) && (*Iterator)->GetIsActivation()) {
				(*Iterator)->Update(DeltaTime);
				Iterator++;
			}
			else {
				(*Iterator)->PoolThisObject(Iterator);
			}
		}
		EnemyMoveProcessing();
	}
}

void EnemyClass::Render(LPD3DXSPRITE Sprite) {
	Pawn::Render(Sprite);
}

void EnemyClass::Destroy() {
	Pawn::Destroy();
}

void EnemyClass::TriggerCollisionEventByOtherActor(Actor* OtherActor) {
	if (OtherActor && OtherActor != this && m_Collision != OtherActor->GetActorCollisionType()) {
		if (OtherActor->GetActorCollisionType() == CT_PLAYER) {

		}
		else if (OtherActor->GetActorCollisionType() == CT_PROJECTILE) {
			ProjectileClass* Projectile = (ProjectileClass*)OtherActor;
			if (Projectile && Projectile->GetOwner()->GetActorCollisionType() != CT_ENEMY) {
				ApplyDamage(10.f);
			}
		}
	}
}

void EnemyClass::ClearObject() {
	m_bIsDead = m_bIsActive = false;
	ClearProjectilePool();
	m_ActivedProjectiles.clear();
}

void EnemyClass::SpawnObject() {
	m_LastSpawnTime = std::chrono::system_clock::now();
	m_Random = std::uniform_real_distribution<float>(m_MinSpawnDelay, m_MaxSpawnDelay);
	m_SpawnTime = std::chrono::duration<float>(m_Random(m_RandomAlgorithm));

	m_Random = std::uniform_real_distribution<float>(m_Texture->GetImageCenter().y * 2, GetWindowSize().bottom - m_Texture->GetImageCenter().y * 2);
	m_Texture->SetPosition(D3DXVECTOR3((FLOAT)GetWindowSize().right + m_Texture->GetImageCenter().x, m_Random(m_RandomAlgorithm), 0.f));

	GetPoolManager()->GetPoolObject("DefaultProjectile", m_Projectiles[PS_DEFAULT], 10);
}

void EnemyClass::PoolThisObject(std::vector<EnemyClass*>::iterator& It) {
	if (It != m_ActivatedList->cend() && GetPoolManager()) {
		(*m_ObjectList)[GetPoolManager()->GetKeyByObjectName(m_Name)].push(*It);
		It = m_ActivatedList->erase(It);
		ClearObject();
	}
}

bool EnemyClass::IsItOutOfScreen() {
	if (m_Texture->GetPosition().x + m_Texture->GetImageCenter().x < GetWindowSize().left || m_Texture->GetPosition().y + m_Texture->GetImageCenter().y < GetWindowSize().top || m_Texture->GetPosition().y - m_Texture->GetImageCenter().y > GetWindowSize().bottom) {
		return true;
	}
	return false;
}

void EnemyClass::OutOfScreen() {
	m_bIsDead = true;
}
