#include "EnemyClass.h"
#include <random>

EnemyClass::EnemyClass() {
}

bool EnemyClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	Pawn::Init(Device, FileSrc, CustomRect);

	std::random_device RandDevice;
	std::mt19937_64 RandAlgo(RandDevice());
	std::uniform_real_distribution<float> Rand(m_Texture->GetImageCenter().y, GetWindowSize().bottom - m_Texture->GetImageCenter().y);

	m_Texture->SetPosition(D3DXVECTOR3((FLOAT)GetWindowSize().right + (FLOAT)m_Texture->GetRect().right * 2.f, Rand(RandAlgo), 0.f));
	return true;
}

void EnemyClass::Update(float DeltaTime) {
	ProcessEnemyMovement();
}

void EnemyClass::Render(LPD3DXSPRITE Sprite) {
	Pawn::Render(Sprite);

}

void EnemyClass::Destroy() {
	Pawn::Destroy();
}