#include "EventClass.h"
#include "TextureClass.h"
#include "SystemClass.h"
#include "ActorClass.h"

EventClass* EventClass::m_Application;

EventClass::EventClass() : m_bIsStop(false) {
	m_EventThread = std::thread([&]() {
		while (m_bIsStop) {
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
		// Actor가 활성화 되어 있을 경우
		if (AActor && AActor->GetIsActivation()) {
			for (auto Iterator = EventIt.first; Iterator != EventIt.second; ++Iterator) {
				Actor* SecondActor = Iterator->second.first;
				// Actor가 활성화 되어 있고, 콜리전 타입이 다를 경우
				if (SecondActor && CheckIt != Iterator && SecondActor->GetIsActivation() && AActor->GetActorCollisionType() != SecondActor->GetActorCollisionType()) {
					// 충돌 확인
					CheckIsColliding(AActor, SecondActor);
				}
			}
		}
	}

	for (auto CheckIt = TriggerIt.first; CheckIt != TriggerIt.second; ++CheckIt) {
		// Actor가 활성화 되어있고, 화면을 나갔을 때 실행할 함수가 있는 경우
		if (CheckIt->second.first->GetIsActivation() && CheckIt->second.second) {
			// 화면을 나갔는지 검사를 하고, 나갔을 경우 함수 실행.
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

	// 현재 충돌을 했는지 검사를 하고, 했을 경우 TriggerCollisionEventByOtherActor함수를 실행.
	if (FirstTexture->GetPosition().x - FirstTexture->GetImageCenter().x < SecondTexture->GetPosition().x + SecondTexture->GetImageCenter().x && FirstTexture->GetPosition().x + FirstTexture->GetImageCenter().x > SecondTexture->GetPosition().x - SecondTexture->GetImageCenter().x && FirstTexture->GetPosition().y - FirstTexture->GetImageCenter().y < SecondTexture->GetPosition().y + SecondTexture->GetImageCenter().y && FirstTexture->GetPosition().y + FirstTexture->GetImageCenter().y > SecondTexture->GetPosition().y - SecondTexture->GetImageCenter().y) {
		First->TriggerCollisionEventByOtherActor(Second);
		Second->TriggerCollisionEventByOtherActor(First);
	}
}