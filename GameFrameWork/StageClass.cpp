#include "StageClass.h"
#include "SystemClass.h"

StageClass::StageClass() {
	m_WindowSize = SystemClass::GetInst()->GetWindowSize();
}

StageClass::~StageClass() {

}

bool StageClass::Init(LPDIRECT3DDEVICE9 Device) {

	return true;
}

void StageClass::Update(float DeltaTime) {

}

void StageClass::Render(LPD3DXSPRITE Sprite) {

}