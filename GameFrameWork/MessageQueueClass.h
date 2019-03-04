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
	// �޽��� ť
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

// ���ø� �������� �Լ��ε�, �ð��� ���ٸ� �˻��� �غ���
// ���ٸ� �׳� �ϱ��ϴ°� ���� �κ�
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
		// ���� ��� ���� ������ �� �ϳ��� ���� �۾��� ó��
		m_Condition.notify_one();
	}
	catch (std::exception Exception) {
	}
}