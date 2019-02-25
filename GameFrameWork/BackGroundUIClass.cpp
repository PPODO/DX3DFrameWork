#include "BackGroundUIClass.h"

bool BackGroundUIClass::Init(LPDIRECT3DDEVICE9 Device, bool bIsMoving, LPCTSTR FileSrc, LPCTSTR FileSrc2) {
	Actor::Init(Device, FileSrc);
	m_Texture->SetImageCenter(D3DXVECTOR3(0.f, 0.f, 0.f));
	if (FileSrc2) {
		m_SecondImage = new TextureClass(Device, FileSrc2);
		if (!m_SecondImage->m_Texture) {
			return false;
		}
		m_SecondImage->SetImageCenter(D3DXVECTOR3(FLOAT(m_SecondImage->m_ImageRect.right) * -1, 0.f, 0.f));
	}
	bIsMovingScreen = bIsMoving;
	return true;
}

void BackGroundUIClass::Update(float DeltaTime) {
	if (bIsMovingScreen) {
		BackgroundImageMoveProcessing(m_Texture);
		if (m_SecondImage) {
			BackgroundImageMoveProcessing(m_SecondImage);
		}
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
