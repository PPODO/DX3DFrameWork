#include "RushEnemy.h"

RushEnemy::RushEnemy() {
	m_Name = "RushEnemy";
	m_XMoveSpeed = 10.f;
	m_YMoveSpeed = 2.5f;
	m_MinSpawnDelay = 1.f;
	m_MaxSpawnDelay = 2.f;
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

	m_Texture->SetPosition(D3DXVECTOR3(m_Texture->GetPosition().x - m_XMoveSpeed, m_Texture->GetPosition().y, 0.f));
}