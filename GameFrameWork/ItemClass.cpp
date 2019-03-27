#include "ItemClass.h"

ItemClass::ItemClass() {
}

ItemClass::~ItemClass() {
}

bool ItemClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	Actor::Init(Device, FileSrc);

	return true;
}

void ItemClass::Update(float DeltaTime, float ActorHeight) {
	Actor::Update(DeltaTime, ActorHeight);
}

void ItemClass::Render(LPD3DXSPRITE Sprite) {
	Actor::Render(Sprite);
}

void ItemClass::SpawnActor(const D3DXVECTOR3& Location) {

}

void ItemClass::CollisionEventBeginByOtherActor(Actor* OtherActor) {
}
