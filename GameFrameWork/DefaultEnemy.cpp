#include "DefaultEnemy.h"

DefaultEnemy::DefaultEnemy() {
	m_MoveSpeed = { -2.5f, 0.f };
	m_Name = "Enemy_Default";
}

DefaultEnemy::~DefaultEnemy() {
}

bool DefaultEnemy::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	EnemyClass::Init(Device, L"Enemy/DefaultEnemy.png");

	return true;
}

void DefaultEnemy::Update(float DeltaTime) {
	EnemyClass::Update(DeltaTime);
}

void DefaultEnemy::Render(LPD3DXSPRITE Sprite) {
	EnemyClass::Render(Sprite);
}

void DefaultEnemy::EnemyMovementProcessing() {
	GetActorImage()->AddXPosition(m_MoveSpeed.x);
}