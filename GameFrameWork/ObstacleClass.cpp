#include "ObstacleClass.h"
#include "EventClass.h"

ObstacleClass::ObstacleClass() {
	m_bIsActivated = false;
	m_CollisionType = ECT_OBSTACLE;

	EventClass::GetInst()->BindCollisionEvent(this);
	EventClass::GetInst()->BindTriggerEvent(this, [this]() { SetActorIsActivated(false); });
}

ObstacleClass::~ObstacleClass() {
}

bool ObstacleClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	Actor::Init(Device, FileSrc);

	return true;
}

void ObstacleClass::Update(float DeltaTime, float ActorHeight) {
	Actor::Update(DeltaTime, ActorHeight);

	if (m_bIsActivated) {
		ObstacleMovementProcessing();
	}
}

void ObstacleClass::Render(LPD3DXSPRITE Sprite) {
	Actor::Render(Sprite);

}

bool ObstacleClass::IsItOutOfScreen() {
	auto m_Image = GetActorImage();
	if (m_Image && m_Image->GetPosition().x + m_Image->GetImageCenter().x <= 0.f || m_Image->GetPosition().x - m_Image->GetImageCenter().x > GetWindowSize().right) {
		return true;
	}
	return false;
}

void ObstacleClass::SpawnActor(const D3DXVECTOR3& Location) {
	GetActorImage()->SetPosition(D3DXVECTOR3(FLOAT(GetWindowSize().right) -GetActorImage()->GetImageCenter().x, Location.y, 0.f));
	SetActorIsActivated(true);
}

void ObstacleClass::ClearObject() {
}
