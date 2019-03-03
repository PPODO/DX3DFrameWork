#pragma once
#include "Actor.h"
#include <thread>
#include <mutex>
#include <unordered_map>
#include <map>
#include <utility>

enum EventCondition { EC_COLLISION, EC_TRIGGER };

class EventClass {
private:
	std::thread m_EventThread;
	std::mutex m_Lock;

	bool m_bIsStop;

private:
	std::unordered_multimap<EventCondition, std::pair<class Actor*, std::function<void()>>> m_Events;

private:
	inline void CheckIsColliding(class Actor* First, class Actor* Second);

public:
	EventClass();
	~EventClass();

	void Frame();

public:
	void BindCollisionEvent(class Actor*);
	void BindTriggerEvent(class Actor*, const std::function<void()>& Function);

};