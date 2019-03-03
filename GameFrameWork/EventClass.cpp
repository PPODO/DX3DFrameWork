#include "EventClass.h"
#include "TextureClass.h"
#include "SystemClass.h"
#include "ActorClass.h"

EventClass::EventClass() : m_bIsStop(false) {
	m_EventThread = std::thread([&]() {
		while (true) {
			if (m_bIsStop) {
				break;
			}
			if (SystemClass::GetInst()->GetActorManager() && SystemClass::GetInst()->GetActorManager()->GetCurrentStage() > 0) {
				Frame();
			}
		}
	});
}

EventClass::~EventClass() {
	m_bIsStop = true;
	m_EventThread.join();
	m_Events.clear();
}

void EventClass::Frame() {
	auto EventIt = m_Events.equal_range(EC_COLLISION);
	auto TriggerIt = m_Events.equal_range(EC_TRIGGER);

	for (auto CheckIt = EventIt.first; CheckIt != EventIt.second; ++CheckIt) {
		Actor* AActor = CheckIt->second.first;
		if (AActor && AActor->GetIsActivation()) {
			for (auto Iterator = EventIt.first; Iterator != EventIt.second; ++Iterator) {
				Actor* SecondActor = Iterator->second.first;
				if (SecondActor && CheckIt != Iterator && SecondActor->GetIsActivation() && AActor->GetActorCollisionType() != SecondActor->GetActorCollisionType()) {
					CheckIsColliding(AActor, SecondActor);
				}
			}
		}
	}

	for (auto CheckIt = TriggerIt.first; CheckIt != TriggerIt.second; ++CheckIt) {
		if (CheckIt->second.first->GetIsActivation() && CheckIt->second.second) {
			if (CheckIt->second.first->IsItOutOfScreen()) {
				CheckIt->second.second();
			}
		}
	}
}

void EventClass::BindCollisionEvent(Actor* EventActor) {
	m_Events.insert(std::make_pair(EC_COLLISION, std::make_pair(EventActor, nullptr)));
}

void EventClass::BindTriggerEvent(Actor* EventActor, const std::function<void()>& Function) {
	m_Events.insert(std::make_pair(EC_TRIGGER, std::make_pair(EventActor, Function)));
}

inline void EventClass::CheckIsColliding(Actor* First, Actor* Second) {
	TextureClass* FirstTexture = First->GetTexture();
	TextureClass* SecondTexture = Second->GetTexture();

	if (FirstTexture->GetPosition().x - FirstTexture->GetImageCenter().x < SecondTexture->GetPosition().x + SecondTexture->GetImageCenter().x && FirstTexture->GetPosition().x + FirstTexture->GetImageCenter().x > SecondTexture->GetPosition().x - SecondTexture->GetImageCenter().x && FirstTexture->GetPosition().y - FirstTexture->GetImageCenter().y < SecondTexture->GetPosition().y + SecondTexture->GetImageCenter().y && FirstTexture->GetPosition().y + FirstTexture->GetImageCenter().y > SecondTexture->GetPosition().y - SecondTexture->GetImageCenter().y) {
		First->TriggerCollisionEventByOtherActor(Second);
		Second->TriggerCollisionEventByOtherActor(First);
	}
}