#include "ActorClass.h"
#include "Actor.h"
#include "PlayerClass.h"

ActorClass::ActorClass() {
}

ActorClass::~ActorClass() {
	for (Actor* AActor : m_Actors) {
		if (AActor) {
			AActor->Destroy();
			delete AActor;
			AActor = nullptr;
		}
	}
	m_Actors.clear();
}

bool ActorClass::Init(LPDIRECT3DDEVICE9 Device) {
	new PlayerClass(m_Actors);

	for (Actor* AActor : m_Actors) {
		if (AActor) {
			AActor->Init(Device);
		}
	}
	return true;
}

void ActorClass::Frame(float DeltaTime) {
	for (Actor* AActor : m_Actors) {
		if (AActor) {
			AActor->Update(DeltaTime);
		}
	}
}