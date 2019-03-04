#include "MessageQueueClass.h"

MessageQueueClass* MessageQueueClass::m_Application;

MessageQueueClass::MessageQueueClass() : m_bIsStop(false) {
	m_QueueThread = std::thread([&]() {
		while (true) {
			// mutex 부분은 https://jacking.tistory.com/1066, unique_lock은 구글 검색 
			std::unique_lock<std::mutex> Lock(m_Lock);
			// 메시지 큐가 비어 있을 경우 스레드를 잠시 멈춤
			while (!m_bIsStop && m_MessageQueue.empty()) {
				// Condition_variable이 스레드를 멈추거나, 특정 상황에서 스레드를 깨워 작업을 처리하게 할 수 있는 이벤트 객체(?)임
				m_Condition.wait(Lock);
			}
			// 메시지 큐가 비어 있고, 스레드를 멈춘 경우
			if (m_bIsStop && m_MessageQueue.empty()) {
				// 루프 탈출
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