#include "DefaultEnemy.h"
#include "EventClass.h"

DefaultEnemy::DefaultEnemy() {
	m_CollisionType = ECT_ENEMY;
	m_ProjectileType = EPT_BOMB;
	m_MoveSpeed = { -2.5f, 0.f };
	m_Name = "Enemy_Default";
	m_Movement = 0.f;

	m_FireDelayTime = std::chrono::duration<float>(0.5f);
	EventClass::GetInst()->BindCollisionEvent(this);
}

DefaultEnemy::~DefaultEnemy() {
}

bool DefaultEnemy::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	EnemyClass::Init(Device, L"Enemy/DefaultEnemy.png");

	return true;
}

void DefaultEnemy::Update(float DeltaTime, float ActorHeight) {
	EnemyClass::Update(DeltaTime, ActorHeight);
}

void DefaultEnemy::Render(LPD3DXSPRITE Sprite) {
	EnemyClass::Render(Sprite);
}

void DefaultEnemy::SpawnActor(const D3DXVECTOR3& Location) {
	EnemyClass::SpawnActor(Location);
}

void DefaultEnemy::EnemyMovementProcessing() {
	m_Movement += 5;

	GetActorImage()->AddPosition(D3DXVECTOR3(1.f, sin(D3DXToRadian(m_Movement)), 0.f) * m_MoveSpeed.x);
}