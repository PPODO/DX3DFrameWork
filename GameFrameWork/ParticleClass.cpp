#include "ParticleClass.h"

ParticleClass::ParticleClass() {
	m_bIsActivated = false;
}

ParticleClass::~ParticleClass() {
}

bool ParticleClass::Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) {
	Actor::Init(Device, FileSrc);
	AnimationClass::Init(GetActorImage());

	return true;
}

void ParticleClass::Update(float DeltaTime, float ActorHeight) {
	if (!AnimationClass::Update(GetActorIsActivated(), GetActorImage())) {
		GetActorImage()->AddYPosition(ActorHeight);
	}
	else {
		m_bIsActivated = false;
	}
}

void ParticleClass::Render(LPD3DXSPRITE Sprite) {
	Actor::Render(Sprite);
}

void ParticleClass::CollisionEventBeginByOtherActor(Actor* OtherActor) {

}

void ParticleClass::SpawnActor(const D3DXVECTOR3& Location) {
	if (GetActorImage()) {
		GetActorImage()->SetPosition(Location);
	}
	SetActorIsActivated(true);
}