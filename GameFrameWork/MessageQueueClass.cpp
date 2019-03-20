#include "MessageQueueClass.h"

MessageQueueClass* MessageQueueClass::m_Application;

MessageQueueClass::MessageQueueClass() : m_bIsStop(false) {
	m_QueueThread = std::thread([&]() {
		while (true) {
			std::unique_lock<std::mutex> Lock(m_Lock);
			while (!m_bIsStop && m_MessageQueue.empty()) {
				m_Condition.wait(Lock);
			}
			if (m_bIsStop && m_MessageQueue.empty()) {
				return;
			}
			Lock.unlock();
		}
	});
}

MessageQueueClass::~MessageQueueClass() {
	{
		std::unique_lock<std::mutex> Lock(m_Lock);
		m_bIsStop = true;
	}
	m_QueueThread.join();
}

std::tuple<MessageState, std::function<void()>> MessageQueueClass::PopMessage() {
	std::tuple<MessageState, std::function<void()>> Task;
	{
		std::unique_lock<std::mutex> Lock(m_Lock);
		Task = m_MessageQueue.front();
		m_MessageQueue.pop();
	}
	return Task;
}