#include "EnemyClass.h"
#include "GraphicClass.h"
#include "SystemClass.h"
#include "InputClass.h"

bool EnemyClass::Init(LPDIRECT3DDEVICE9 Device, LPCWSTR ImageSrc, bool bUseCustomRect, RECT CustomRect) {
	Actor::Init(Device, L"Enemy.png");

	return true;
}

void EnemyClass::Update() {

}

void EnemyClass::Render(LPD3DXSPRITE Sprite) {
	Actor::Render(Sprite);

}

void EnemyClass::Destroy() {

}