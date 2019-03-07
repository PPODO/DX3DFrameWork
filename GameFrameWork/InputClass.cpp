#include "InputClass.h"

bool operator==(const RECT& R1, const RECT& R2) {
	return (R1.right == R2.right ? R1.left == R2.left ? R1.bottom == R2.bottom ? R1.top == R2.top ? true : false : false : false : false);
}

InputClass::InputClass() : m_MouseX(0), m_MouseY(0), m_MouseActionDataSave(nullptr) {
}

InputClass::~InputClass() {
	m_ActionKeys.clear();
	m_AxisKeys.clear();
	m_MouseActions.clear();
}

void InputClass::Frame() {
	// Axis Ű�� ��� ���� �ʾ��� ���
	if (!m_AxisKeys.empty()) {
		for (auto AxisKey : m_AxisKeys) {
			// ���� Ű�� ������ �ִ���
			if (std::get<0>(AxisKey.second)) {
				// ������ �ִٸ� �Լ��� ����
				std::get<1>(AxisKey.second)();
			}
		}
	}
}

void InputClass::KeyIsDown(unsigned short Key) {
	CheckBindKeys(Key, IE_Pressed, true);
}

void InputClass::KeyIsUp(unsigned short Key) {
	CheckBindKeys(Key, IE_Released, false);
}

void InputClass::MouseIsDown(unsigned short Key, short X, short Y) {
	// Key�� ������ ���� ������ �ִ� �������� ������ ���ؿ�.
	auto Iterator = m_MouseActions.equal_range(Key);
	// ã���� ���
	if (Iterator.first != m_MouseActions.cend()) {
		// ������ ������ ó������ ������ ����
		for (auto It = Iterator.first; It != Iterator.second; ++It) {
			// ���� ���콺 ��ǥ�� �������Ǵ� ��ư�� �ִ��� �˻�
			if (CheckPosInRect(std::get<0>(It->second), X, Y)) {
				m_MouseActionDataSave = &It->second;
				// ��ư ������ ����
				std::get<1>(*m_MouseActionDataSave)();
				break;
			}
		}
	}
}

void InputClass::MouseIsUp(unsigned short Key, short X, short Y) {
	// Key�� ������ ���� ������ �ִ� �������� ������ ���ؿ�.
	auto Iterator = m_MouseActions.equal_range(Key);
	// ã���� ���
	if (Iterator.first != m_MouseActions.cend()) {
		// ������ ������ ó������ ������ ����
		for (auto It = Iterator.first; It != Iterator.second; ++It) {
			// ���� ���콺 ��ǥ�� �������Ǵ� ��ư�� �ִ��� �˻�
			if (CheckPosInRect(std::get<0>(It->second), X, Y) && std::get<0>(It->second) == std::get<0>(*m_MouseActionDataSave)) {
				// ��ư�� �������� ���� ������ �Լ��� ȣ����.
				std::get<2>(*m_MouseActionDataSave)();
				break;
			}
		}
		if (m_MouseActionDataSave) {
			// ������ ������ �ٲ� ��ư�� �����Ѵٸ�, ������ ������� ����
			std::get<1>(*m_MouseActionDataSave)();
		}
	}
	m_MouseActionDataSave = nullptr;
}

void InputClass::BindAxisDelegate(unsigned short Key, Function Delegate) {
	m_AxisKeys.insert(std::make_pair(Key, std::make_tuple(false, Delegate)));
}

void InputClass::BindActionDelegate(unsigned short Key, InputState IS, Function Delegate) {
	m_ActionKeys.insert(std::make_pair(Key, std::make_tuple(IS, Delegate)));
}

void InputClass::BindMouseActionDelegate(unsigned short Key, RECT Rect, Function Delegate, Function DelegateTwo) {
	m_MouseActions.insert(std::pair<unsigned short, std::tuple<RECT, Function, Function>>(Key, std::tuple<RECT, Function, Function>(Rect, Delegate, DelegateTwo)));
}

void InputClass::SetMousePosition(short X, short Y) {
	m_MouseX = X;
	m_MouseY = Y;
}