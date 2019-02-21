#include "PlayerClass.h"

PlayerClass::PlayerClass(std::vector<Actor*>& Vector) : Actor(Vector) {
}

bool PlayerClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) {
	Actor::Init(Device, L"Player.png", CustomRect);

	return true;
}

void PlayerClass::Update(float DeltaTime) {

}

void PlayerClass::Destroy() {

}