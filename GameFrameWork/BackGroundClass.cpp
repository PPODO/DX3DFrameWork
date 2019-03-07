#include "BackGroundClass.h"

BackGroundClass::BackGroundClass() : m_ImageMoveSpeed(0.f), m_bIsMovingScreen(false), m_MoveDirection(0) {
	m_Collision = CT_NONE;
	m_ImagePos = D3DXVECTOR3(0.f, 0.f, 0.f);
}

void BackGroundClass::Update(float DeltaTime) {
	for (const auto& It : m_BackGroundImages) {
		if (It) {
			if (m_bIsMovingScreen) {
				BackgroundImageMoveProcessing(It->GetTexture());
			}
			It->Update(DeltaTime);
		}
	}
}

void BackGroundClass::Render(LPD3DXSPRITE Sprite) {
	for (const auto& It : m_BackGroundImages) {
		if (It) {
			It->Render(Sprite);
		}
	}
}

void BackGroundClass::Destroy() {
	for (auto& It : m_BackGroundImages) {
		if (It) {
			It->Destroy();
			delete It;
		}
	}
}

void BackGroundClass::SetCollisionTypeAndBindEvent(CollisionType CT) {
	for (auto& It : m_BackGroundImages) {
		if (It) {
			It->SetCollisionType(CT);
			EventClass::GetInst()->BindCollisionEvent(It);
		}
	}
}