#include "EnemyClass.h"
#include "SystemClass.h"
#include "ActorClass.h"
#include "DefaultProjectileClass.h"

std::random_device EnemyClass::m_RandomDevice;
std::mt19937_64 EnemyClass::m_RandomAlgorithm;
std::uniform_real_distribution<float> EnemyClass::m_Random;

std::vector<std::stack<EnemyClass*>>* EnemyClass::m_ObjectList = nullptr;
std::vector<EnemyClass*>* EnemyClass::m_ActivatedList = nullptr;

TextureClass* EnemyClass::m_Target = nullptr;

EnemyClass::EnemyClass() {
	m_Projectiles.resize(PS_DEFAULT + 1);

	m_MaxActivatedProjectile = 5;
	m_RandomAlgorithm = std::mt19937_64(m_RandomDevice());
	m_FireDelay = std::chrono::duration<float>(0.25f);
	m_bIsActive = false;
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
		for (auto Iterator = m_ActivedProjectiles.begin(); Iterator != m_ActivedProjectiles.end();) {
			if (!(*Iterator)->CheckColliding(Iterator, std::bind(&EnemyClass::IsProjectileOutOfScreen, this, *Iterator))) {
				(*Iterator)->Update(DeltaTime);
				Iterator++;
			}
		}
		EnemyMoveProcessing();
		SpawnProjectile(D3DXVECTOR3(-1.f, 0.f, 0.f));
	}
}

void EnemyClass::Render(LPD3DXSPRITE Sprite) {
	Pawn::Render(Sprite);

}

void EnemyClass::Destroy() {
	Pawn::Destroy();
}

void EnemyClass::ClearObject() {
	ClearProjectilePool();
	m_bIsActive = false;
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
		ClearObject();

		(*m_ObjectList)[GetPoolManager()->GetKeyByObjectName(m_Name)].push(*It);
		It = m_ActivatedList->erase(It);
	}
}

bool EnemyClass::IsProjectileOutOfScreen(ProjectileClass* ProjectileObject) {
	if (ProjectileObject->GetTexture()->GetPosition().x + ProjectileObject->GetTexture()->GetImageCenter().x < GetWindowSize().left) {
		return true;
	}
	return false;
}