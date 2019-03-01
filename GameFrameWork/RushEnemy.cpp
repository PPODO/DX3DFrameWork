#include "RushEnemy.h"

RushEnemy::RushEnemy() {

	m_Name = "RushEnemy";
	m_XMoveSpeed = 10.f;
	m_YMoveSpeed = 2.5f;
	m_MinSpawnDelay = 3.f;
	m_MaxSpawnDelay = 5.f;
}

bool RushEnemy::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	EnemyClass::Init(Device, L"RushEnemy.png");

	return true;
}

void RushEnemy::Update(float DeltaTime) {
	EnemyClass::Update(DeltaTime);

}

void RushEnemy::Render(LPD3DXSPRITE Sprite) {
	EnemyClass::Render(Sprite);

}

void RushEnemy::Destroy() {
	EnemyClass::Destroy();
}

void RushEnemy::EnemyMoveProcessing() {
	m_Texture->AddPosition(m_MoveDirection * m_XMoveSpeed);
}

void RushEnemy::SpawnObject() {
	EnemyClass::SpawnObject();

	m_MoveDirection = m_Target->GetPosition() - m_Texture->GetPosition();
	D3DXVec3Normalize(&m_MoveDirection, &m_MoveDirection);
}