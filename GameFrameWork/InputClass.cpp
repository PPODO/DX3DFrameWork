#include "InputClass.h"

InputClass::InputClass() : m_MouseX(0), m_MouseY(0), m_MouseActionDataSave(nullptr) {
}

InputClass::~InputClass() {
	m_ActionKeys.clear();
	m_AxisKeys.clear();
	m_MouseActions.clear();
}

void InputClass::Frame() {
	// Axis 키가 비어 있지 않았을 경우
	if (!m_AxisKeys.empty()) {
		for (auto AxisKey : m_AxisKeys) {
			// 현재 키가 눌러져 있는지
			if (std::get<0>(AxisKey.second)) {
				// 눌러져 있다면 함수를 실행
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
	auto Iterator = m_MouseActions.equal_range(Key);
	if (Iterator.first != m_MouseActions.cend()) {
		for (auto It = Iterator.first; It != Iterator.second; ++It) {
			if (CheckPosInRect(std::get<0>(It->second), X, Y)) {
				m_MouseActionDataSave = &It->second;
				std::get<1>(*m_MouseActionDataSave)();
				break;
			}
		}
	}
}

void InputClass::MouseIsUp(unsigned short Key, short X, short Y) {
	auto Iterator = m_MouseActions.equal_range(Key);
	if (Iterator.first != m_MouseActions.cend()) {
		for (auto It = Iterator.first; It != Iterator.second; ++It) {
			if (CheckPosInRect(std::get<0>(It->second), X, Y)) {
				std::get<2>(*m_MouseActionDataSave)();
				break;
			}
		}
		if (m_MouseActionDataSave) {
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
	m_MouseActions.insert(std::make_pair(Key, std::make_tuple(Rect, Delegate, DelegateTwo)));
}

void InputClass::SetMousePosition(short X, short Y) {
	m_MouseX = X;
	m_MouseY = Y;
}