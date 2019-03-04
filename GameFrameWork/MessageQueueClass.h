#pragma once
#include "Singleton.h"
#include "SystemClass.h"
#include <queue>
#include <thread>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <tuple>
#include <future>
#include <Windows.h>

enum MessageState { MS_INIT, MS_RENDER, MS_DESTROY };

class MessageQueueClass : public Singleton<MessageQueueClass> {
private:
	// 메시지 큐
	std::queue<std::tuple<MessageState, std::function<void()>>> m_MessageQueue;
	bool m_bIsStop;
	std::thread m_QueueThread;
	std::condition_variable m_Condition;
	std::mutex m_Lock;

public:
	MessageQueueClass();
	~MessageQueueClass();
	
	template<typename F, typename ...Argc>
	void PushMessage(MessageState Message, F&&, Argc&&...);
	std::tuple<MessageState, std::function<void()>> PopMessage();

	bool IsEmpty() const { return m_MessageQueue.empty(); }
};

// 템플릿 가변인자 함수인데, 시간이 많다면 검색을 해보고
// 없다면 그냥 암기하는게 편한 부분
template<typename F, typename ...Argc>
inline void MessageQueueClass::PushMessage(MessageState Message, F&& Function, Argc&&... _Argc) {
	try {
		if (m_bIsStop) {
			throw std::runtime_error("Message Queue Thread is Stop!");
		}

		{
			std::unique_lock<std::mutex> Lock(m_Lock);
			m_MessageQueue.push(std::make_tuple(Message, std::bind(std::forward<F>(Function), std::forward<Argc>(_Argc)...)));
		}
		// 현재 대기 중인 스레드 중 하나를 깨워 작업을 처리
		m_Condition.notify_one();
	}
	catch (std::exception Exception) {
	}
}