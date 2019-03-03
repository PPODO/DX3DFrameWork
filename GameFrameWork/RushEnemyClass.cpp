#include "RushEnemyClass.h"

RushEnemyClass::RushEnemyClass() : m_bSetDirection(false) {
	m_Name = "RushEnemy";
	m_UseAutoSpawn = false;
	m_XMoveSpeed = 12.5f;
	m_MinSpawnDelay = 1.f;
	m_MaxSpawnDelay = 4.f;
}

RushEnemyClass::~RushEnemyClass() {
}

bool RushEnemyClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	EnemyClass::Init(Device, L"RushEnemy.png");

	return true;
}

void RushEnemyClass::Update(float DeltaTime) {
	EnemyClass::Update(DeltaTime);

	if (m_bIsActive && !m_bSetDirection) {
		CalculateDirection();
		m_bSetDirection = true;
	}
}

void RushEnemyClass::Render(LPD3DXSPRITE Sprite) {
	EnemyClass::Render(Sprite);
}

void RushEnemyClass::Destroy() {
	EnemyClass::Destroy();
}

void RushEnemyClass::EnemyMoveProcessing() {
	if (m_bSetDirection) {
		m_Texture->AddPosition(MoveDirection * m_XMoveSpeed);
	}
}

void RushEnemyClass::ClearObject() {
	EnemyClass::ClearObject();
	m_bSetDirection = false;
}

void RushEnemyClass::CalculateDirection() {
	MoveDirection = m_Target->GetPosition() - m_Texture->GetPosition();
	D3DXVec3Normalize(&MoveDirection, &MoveDirection);
}