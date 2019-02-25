#include "InputClass.h"

InputClass::InputClass() : m_MouseX(0), m_MouseY(0), m_MouseActionDataSave(nullptr) {
}

InputClass::~InputClass() {
	m_ActionKeys.clear();
	m_AxisKeys.clear();
	m_MouseActions.clear();
}

void InputClass::Frame() {
	if (!m_AxisKeys.empty()) {
		for (auto AxisKey : m_AxisKeys) {
			if (std::get<0>(AxisKey.second)) {
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
	auto Iterator = m_MouseActions.find(Key);
	if (Iterator != m_MouseActions.cend() && CheckPosInRect(std::get<0>(Iterator->second), X, Y)) {
		m_MouseActionDataSave = &Iterator->second;
		std::get<1>(*m_MouseActionDataSave)();
	}
}

void InputClass::MouseIsUp(unsigned short Key, short X, short Y) {
	auto Iterator = m_MouseActions.find(Key);
	if (Iterator != m_MouseActions.cend()) {
		if (CheckPosInRect(std::get<0>(Iterator->second), X, Y)) {
			std::get<2>(*m_MouseActionDataSave)();
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

void InputClass::BindActionDelegate(unsigned short Key, Function Delegate) {
	m_ActionKeys.insert(std::make_pair(Key, std::make_tuple(IE_None, Delegate)));
}

void InputClass::BindMouseActionDelegate(unsigned short Key, RECT Rect, Function Delegate, Function DelegateTwo) {
	m_MouseActions.insert(std::make_pair(Key, std::make_tuple(Rect, Delegate, DelegateTwo)));
}

void InputClass::SetMousePosition(short X, short Y) {
	m_MouseX = X;
	m_MouseY = Y;
}