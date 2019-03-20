#include "ObstacleClass.h"

ObstacleClass::ObstacleClass() {
	m_bIsActivated = false;
	m_CollisionType = ECT_OBSTACLE;
	m_MoveSpeed = { 0.f, 0.f };
}

ObstacleClass::~ObstacleClass() {
}

bool ObstacleClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	Actor::Init(Device, FileSrc);

	return true;
}

void ObstacleClass::Update(float DeltaTime) {
	Actor::Update(DeltaTime);

}

void ObstacleClass::Render(LPD3DXSPRITE Sprite) {
	Actor::Render(Sprite);

}