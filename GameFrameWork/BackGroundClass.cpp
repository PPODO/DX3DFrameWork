#include "BackGroundClass.h"
#include "EventClass.h"

BackGroundClass::BackGroundClass() {
	m_AdditionalSpeed = { 0.f, 0.f };
	m_DefaultMoveSpeed = { 10.f, 0.f };
	m_bIsActivated = true;
}

BackGroundClass::~BackGroundClass() {
}

bool BackGroundClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	Actor::Init(Device, FileSrc);

	return true;
}

void BackGroundClass::Update(float DeltaTime, float ActorHeight) {
	Actor::Update(DeltaTime, ActorHeight);

	CalculateMoveSpeed();
	BackGroundMovementProcessing();
}

void BackGroundClass::Render(LPD3DXSPRITE Sprite) {
	Actor::Render(Sprite);
}

void BackGroundClass::CollisionEventBeginByOtherActor(Actor* OtherActor) {
	if (OtherActor && OtherActor != this && OtherActor->GetActorCollisionType() != m_CollisionType) {

	}
}

void BackGroundClass::ClearObject() {
}

void BackGroundClass::CalculateMoveSpeed() {
	D3DXVec2Lerp(&m_MoveSpeed, &m_DefaultMoveSpeed, &(m_AdditionalSpeed + m_DefaultMoveSpeed), 0.4f);
}

void BackGroundClass::BackGroundMovementProcessing() {
	if (GetActorImage()->GetPosition().x < GetActorImage()->GetImageCenter().x * -1 && m_OtherActor) {
		GetActorImage()->SetPosition(D3DXVECTOR3(m_OtherActor->GetActorImage()->GetPosition().x + FLOAT(m_OtherActor->GetActorImage()->GetRect().right) - m_MoveSpeed.x, GetActorImage()->GetPosition().y, 0.f));
	}
	GetActorImage()->AddXPosition(m_MoveSpeed.x * -1);
}