#include "BackGroundUIClass.h"

bool BackGroundUIClass::Init(LPDIRECT3DDEVICE9 Device, bool bIsMoving, LPCTSTR FileSrc, LPCTSTR FileSrc2) {
	Actor::Init(Device, FileSrc);
	m_Texture->m_ImageCenter = { 0.f, 0.f, 0.f };
	if (FileSrc2) {
		m_SecondImage = new TextureClass(Device, FileSrc2);
		m_SecondImage->m_ImageCenter = { FLOAT(m_Texture->m_ImageRect.right), 0.f, 0.f };
	}

	bIsMovingScreen = bIsMoving;
	return true;
}

void BackGroundUIClass::Update(float DeltaTime) {
	if (bIsMovingScreen) {
		m_Texture->m_ImagePosition.x -= ImageMoveSpeed * DeltaTime;
	}
}

void BackGroundUIClass::Render(LPD3DXSPRITE Sprite) {
	if (m_SecondImage) {
		m_SecondImage->Render(Sprite);
	}
	m_Texture->Render(Sprite);
}

void BackGroundUIClass::Destroy() {
	if (m_SecondImage) {
		delete m_SecondImage;
		m_SecondImage = nullptr;
	}
}
