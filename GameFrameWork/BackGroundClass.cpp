#include "BackGroundClass.h"
#include "EventClass.h"

BackGroundClass::BackGroundClass() {
	m_MoveSpeed = { 5.f, 0.f };
	m_bIsActivated = true;
}

BackGroundClass::~BackGroundClass() {
}

bool BackGroundClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	Actor::Init(Device, FileSrc);

	return true;
}

void BackGroundClass::Update(float DeltaTime) {
	Actor::Update(DeltaTime);

	BackGroundMovementProcessing();
}

void BackGroundClass::Render(LPD3DXSPRITE Sprite) {
	Actor::Render(Sprite);
}

void BackGroundClass::CollisionEventBeginByOtherActor(Actor* OtherActor) {
	if (OtherActor && OtherActor != this && OtherActor->GetActorCollisionType() != m_CollisionType) {

	}
}

void BackGroundClass::BackGroundMovementProcessing() {
	if (GetActorImage()->GetPosition().x < GetActorImage()->GetImageCenter().x * -1) {
		GetActorImage()->SetPosition(D3DXVECTOR3((GetActorImage()->GetRect().right - m_MoveSpeed.x) + GetActorImage()->GetImageCenter().x, GetActorImage()->GetPosition().y, 0.f));
	}
	GetActorImage()->AddXPosition(m_MoveSpeed.x * -1);
}