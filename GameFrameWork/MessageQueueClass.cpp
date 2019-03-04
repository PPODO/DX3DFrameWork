#include "MessageQueueClass.h"

MessageQueueClass* MessageQueueClass::m_Application;

MessageQueueClass::MessageQueueClass() : m_bIsStop(false) {
	m_QueueThread = std::thread([&]() {
		while (true) {
			// mutex �κ��� https://jacking.tistory.com/1066, unique_lock�� ���� �˻� 
			std::unique_lock<std::mutex> Lock(m_Lock);
			// �޽��� ť�� ��� ���� ��� �����带 ��� ����
			while (!m_bIsStop && m_MessageQueue.empty()) {
				// Condition_variable�� �����带 ���߰ų�, Ư�� ��Ȳ���� �����带 ���� �۾��� ó���ϰ� �� �� �ִ� �̺�Ʈ ��ü(?)��
				m_Condition.wait(Lock);
			}
			// �޽��� ť�� ��� �ְ�, �����带 ���� ���
			if (m_bIsStop && m_MessageQueue.empty()) {
				// ���� Ż��
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