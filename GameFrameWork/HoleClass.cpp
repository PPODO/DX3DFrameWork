#include "HoleClass.h"

HoleClass::HoleClass() {
	m_MoveSpeed = { 5.f, 0.f };
	m_Name = "Obstacle_Hole";
}

HoleClass::~HoleClass() {
}

bool HoleClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	ObstacleClass::Init(Device, L"Obstacle/Hole.png");

	return true;
}

void HoleClass::Update(float DeltaTime, float ActorHeight) {
	ObstacleClass::Update(DeltaTime, ActorHeight);
	
}

void HoleClass::Render(LPD3DXSPRITE Sprite) {
	ObstacleClass::Render(Sprite);

}

void HoleClass::CollisionEventBeginByOtherActor(Actor* OtherActor) {
	if (OtherActor && OtherActor != this && OtherActor->GetActorCollisionType() != m_CollisionType) {
		if (OtherActor->GetActorCollisionType() == ECT_PROJECTILE) {

		}
		else if (OtherActor->GetActorCollisionType() == ECT_PLAYER) {
			SetActorIsActivated(false);
		}
	}
}

void HoleClass::ObstacleMovementProcessing() {
	GetActorImage()->AddXPosition(m_MoveSpeed.x * -1);
}