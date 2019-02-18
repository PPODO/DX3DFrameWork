#include "EnemyClass.h"
#include "GraphicClass.h"
#include "SystemClass.h"
#include "InputClass.h"

bool EnemyClass::Init(LPDIRECT3DDEVICE9 Device, LPCWSTR ImageSrc, bool bUseCustomRect, RECT CustomRect) {
	SystemClass::GetInst()->GetInputClass()->BindDelegate(VK_RIGHT, std::bind(&EnemyClass::MoveRight, this, 1.f), 1.f);

	Actor::Init(Device, L"Enemy.png");
	return true;
}

void EnemyClass::Update() {

}

void EnemyClass::Render() {

}

void EnemyClass::Destroy() {

}

void EnemyClass::MoveRight(float Value) {
	m_Image->m_Position.x += Value * 2;
}