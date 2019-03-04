#include "BackGroundClass.h"

bool BackGroundClass::Init(LPDIRECT3DDEVICE9 Device, bool bIsMoving, LPCTSTR FileSrc, LPCTSTR FileSrc2) {
	Actor::Init(Device, FileSrc);

	m_Texture->SetImageCenter(D3DXVECTOR3(0.f, 0.f, 0.f));
	if (FileSrc2) {
		m_SecondImage = new TextureClass(Device, FileSrc2);
		if (!m_SecondImage->m_Texture) {
			return false;
		}
		m_SecondImage->SetImageCenter(D3DXVECTOR3(0.f, 0.f, 0.f));
		m_SecondImage->SetPosition(D3DXVECTOR3((FLOAT)m_SecondImage->GetRect().right, 0.f, 0.f));
	}
	bIsMovingScreen = bIsMoving;
	return true;
}

void BackGroundClass::Update(float DeltaTime) {
	Actor::Update(DeltaTime);

	if (bIsMovingScreen) {
		BackgroundImageMoveProcessing(m_Texture);
		if (m_SecondImage) {
			BackgroundImageMoveProcessing(m_SecondImage);
		}
	}
}

void BackGroundClass::Render(LPD3DXSPRITE Sprite) {
	Actor::Render(Sprite);

	if (m_SecondImage) {
		m_SecondImage->Render(Sprite);
	}
}

void BackGroundClass::Destroy() {
	Actor::Destroy();

	if (m_SecondImage) {
		delete m_SecondImage;
		m_SecondImage = nullptr;
	}
}
