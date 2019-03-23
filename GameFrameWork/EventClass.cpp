#include "EventClass.h"
#include "TextureClass.h"
#include "SystemClass.h"
#include "ActorClass.h"

EventClass* EventClass::m_Application;

EventClass::EventClass() : m_bIsStop(false) {
	m_EventThread = std::thread([&]() {
		{
			std::unique_lock<std::mutex> Lock(m_Lock);
			m_Condition.wait(Lock);
		}
		while (!m_bIsStop) {
			if (SystemClass::GetInst()->GetActorManager() && SystemClass::GetInst()->GetActorManager()->GetCurrentStage() > 0) {
				Frame();
			}
		}
	});
}

EventClass::~EventClass() {
	m_bIsStop = true;
	m_Condition.notify_all();
	m_EventThread.join();
	m_Events.clear();
}

void EventClass::Frame() {
	auto EventIt = m_Events.equal_range(EC_COLLISION);
	auto TriggerIt = m_Events.equal_range(EC_TRIGGER);

	for (auto CheckIt = EventIt.first; CheckIt != EventIt.second; ++CheckIt) {
		Actor* AActor = CheckIt->second.first;
		if (AActor && AActor->GetActorIsActivated() && AActor->GetActorCollisionType() != ECT_NONE) {
			for (auto Iterator = EventIt.first; Iterator != EventIt.second; ++Iterator) {
				Actor* SecondActor = Iterator->second.first;
				if (SecondActor) {
					if (SecondActor->GetActorCollisionType() != ECT_NONE && CheckIt != Iterator) {
						if (SecondActor->GetActorIsActivated()) {
							if (AActor->GetActorCollisionType() != SecondActor->GetActorCollisionType()) {
								CheckIsColliding(AActor, SecondActor);
							}
						}
					}
				}
			}
		}
	}

	for (auto CheckIt = TriggerIt.first; CheckIt != TriggerIt.second; ++CheckIt) {
		if (CheckIt->second.first->GetActorIsActivated() && CheckIt->second.second) {
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
	TextureClass* FirstTexture = First->GetActorImage();
	TextureClass* SecondTexture = Second->GetActorImage();

	if (FirstTexture->GetPosition().x - FirstTexture->GetImageCenter().x <= SecondTexture->GetPosition().x + SecondTexture->GetImageCenter().x && FirstTexture->GetPosition().x + FirstTexture->GetImageCenter().x >= SecondTexture->GetPosition().x - SecondTexture->GetImageCenter().x && FirstTexture->GetPosition().y - FirstTexture->GetImageCenter().y <= SecondTexture->GetPosition().y + SecondTexture->GetImageCenter().y && FirstTexture->GetPosition().y + FirstTexture->GetImageCenter().y >= SecondTexture->GetPosition().y - SecondTexture->GetImageCenter().y) {
		First->CollisionEventBeginByOtherActor(Second);
		Second->CollisionEventBeginByOtherActor(First);
	}
}