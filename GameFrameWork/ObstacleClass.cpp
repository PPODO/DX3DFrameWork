#include "ObstacleClass.h"
#include "EventClass.h"

ObstacleClass::ObstacleClass() {
	m_bIsActivated = false;
	m_CollisionType = ECT_OBSTACLE;

	EventClass::GetInst()->BindTriggerEvent(this, [this]() { SetActorIsActivated(false); });
}

ObstacleClass::~ObstacleClass() {
}

bool ObstacleClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	Actor::Init(Device, FileSrc);

	return true;
}

void ObstacleClass::Update(float DeltaTime) {
	Actor::Update(DeltaTime);

	if (m_bIsActivated) {
		ObstacleMovementProcessing();
	}
}

void ObstacleClass::Render(LPD3DXSPRITE Sprite) {
	Actor::Render(Sprite);

}

bool ObstacleClass::IsItOutOfScreen() {
	auto m_Image = GetActorImage();
	if (m_Image && m_Image->GetPosition().x + m_Image->GetImageCenter().x <= 0.f || m_Image->GetPosition().x - m_Image->GetImageCenter().x > GetWindowSize().right || m_Image->GetPosition().y + m_Image->GetImageCenter().y <= 0.f || m_Image->GetPosition().y - m_Image->GetImageCenter().y > GetWindowSize().bottom) {
		return true;
	}
	return false;
}