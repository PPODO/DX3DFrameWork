#pragma once
#include "Singleton.h"
#include "Queue.h"
#include "SystemClass.h"
#include <queue>
#include <thread>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <tuple>
#include <future>
#include <Windows.h>

enum MessageState { MS_RENDER, MS_DESTROY };

static const int MaxQueueSize = 50;

class MessageQueueClass : public Singleton<MessageQueueClass> {
private:
	std::queue<std::tuple<MessageState, std::function<void()>>> m_MessageQueue;
	bool m_bIsStop;
	std::thread m_QueueThread;
	std::condition_variable m_Condition;
	std::mutex m_Lock;

public:
	MessageQueueClass();
	~MessageQueueClass();
	
	template<typename F, typename ...Argc>
	void PushMessage(MessageState Message, F&& Function, Argc&&... _Argc);
	std::tuple<MessageState, std::function<void()>> PopMessage();

	bool IsEmpty() const { return m_MessageQueue.empty(); }
};

template<typename F, typename ...Argc>
inline void MessageQueueClass::PushMessage(MessageState Message, F&& Function, Argc&& ..._Argc) {
	typedef decltype(std::forward<F>(Function)(std::forward<Argc>(_Argc)...)) ReturnType;

	try {
		if (m_bIsStop) {
			throw std::runtime_error("Message Queue Thread is Stop!");
		}

		{
			std::unique_lock<std::mutex> Lock(m_Lock);
			m_MessageQueue.push(std::make_tuple(Message, std::bind(std::forward<F>(Function), std::forward<Argc>(_Argc)...)));
		}
		m_Condition.notify_all();
	}
	catch (std::exception Exception) {
		//MessageBox(SystemClass::GetInst()->GetWindowHandle(), Exception.what(), L"Error!!", MB_OK);
	}
}