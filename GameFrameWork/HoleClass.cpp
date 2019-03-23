#include "HoleClass.h"

HoleClass::HoleClass() {
	m_MoveSpeed = { 5.f, 0.f };
	m_Name = "Obstacle_Hole";
}

HoleClass::~HoleClass() {
}

bool HoleClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	ObstacleClass::Init(Device, L"Obstacle/Hole.png");

	return true;
}

void HoleClass::Update(float DeltaTime) {
	ObstacleClass::Update(DeltaTime);
	
}

void HoleClass::Render(LPD3DXSPRITE Sprite) {
	ObstacleClass::Render(Sprite);

}

void HoleClass::CollisionEventBeginByOtherActor(Actor* OtherActor) {

}

void HoleClass::ObstacleMovementProcessing() {
	GetActorImage()->AddXPosition(m_MoveSpeed.x * -1);
}