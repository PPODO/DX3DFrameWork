#include "Weapon.h"

Weapon::Weapon() {
	m_bIsActivated = true;
}

Weapon::~Weapon() {
}

bool Weapon::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	Actor::Init(Device, FileSrc);

	return true;
}

void Weapon::Update(float DeltaTime) {

}

void Weapon::Render(LPD3DXSPRITE Sprite) {
	Actor::Render(Sprite);
}

void Weapon::CollisionEventBeginByOtherActor(Actor *) {
}
