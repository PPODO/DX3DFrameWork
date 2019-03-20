#include "PlayerClass.h"
#include "SystemClass.h"
#include "InputClass.h"
#include "EventClass.h"

PlayerClass::PlayerClass() {
	m_CollisionType = ECT_PLAYER;
	EventClass::GetInst()->BindCollisionEvent(this);
	m_LocationIsHaveToGo = FLOAT(((GetWindowSize().right / 2) / 2) / 2);
	m_MoveSpeed = { 5.f, 0.f };
}

PlayerClass::~PlayerClass() {
}

bool PlayerClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	PawnClass::Init(Device, L"Player/Player.png");

	return true;
}

void PlayerClass::SetupPlayerInput() {
	SystemClass::GetInst()->GetInputManager()->BindActionDelegate(VK_UP, IE_Pressed, std::bind(&PlayerClass::Jump, this));

	SystemClass::GetInst()->GetInputManager()->BindAxisDelegate(VK_LEFT, std::bind(&PlayerClass::MoveRight, this, std::placeholders::_1), -1.f);
	SystemClass::GetInst()->GetInputManager()->BindAxisDelegate(VK_RIGHT, std::bind(&PlayerClass::MoveRight, this, std::placeholders::_1), 1.f);
}

void PlayerClass::Update(float DeltaTime) {
	PawnClass::Update(DeltaTime);

	if (!GetStartMoveToLocation()) {

	}
}

void PlayerClass::Render(LPD3DXSPRITE Sprite) {
	PawnClass::Render(Sprite);

}

void PlayerClass::CollisionEventBeginByOtherActor(Actor* OtherActor) {
	if (OtherActor && OtherActor != this && OtherActor->GetActorCollisionType() != m_CollisionType) {
		if (OtherActor->GetActorCollisionType() == ECT_ALLBLOCK) {
			if (!GetIsJumping() && !GetIsLanded()) {
				IsLanded();
			}
		}
	}
}

void PlayerClass::Jump() {
	DoJump();
}

void PlayerClass::MoveRight(float Value) {
	if (!GetStartMoveToLocation() && GetActorImage()) {
		GetActorImage()->AddXPosition(m_MoveSpeed.x * Value);
	}
}

void PlayerClass::PlayStartMoveToLocation() {
	D3DXVECTOR3 NewLocation, ToGoLocation(m_LocationIsHaveToGo, GetActorImage()->GetPosition().y, 0.f);
	D3DXVec3Lerp(&NewLocation, &GetActorImage()->GetPosition(), &ToGoLocation, 0.065f);
	GetActorImage()->SetPosition(NewLocation);

	if (fabs(NewLocation.x - m_LocationIsHaveToGo) <= FLOATNearlyEqual) {
		SetStartMoveToLocation(false);
	}
}