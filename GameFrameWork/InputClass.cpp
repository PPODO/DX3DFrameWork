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
	if (!m_AxisKeys.empty()) {
		std::string AxisName = "";
		for (auto AxisKey : m_AxisKeys) {
			if (GetAsyncKeyState(AxisKey.first) & 0x8000) {
				AxisName = std::get<0>(AxisKey.second);
				std::get<1>(AxisKey.second)(std::get<2>(AxisKey.second));
			}
			else if(AxisName.compare(std::get<0>(AxisKey.second)) != 0) {
				std::get<1>(AxisKey.second)(0.f);
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
			if (m_MouseActionDataSave && CheckPosInRect(std::get<0>(It->second), X, Y) && std::get<0>(It->second) == std::get<0>(*m_MouseActionDataSave)) {
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

void InputClass::BindAxisDelegate(unsigned short Key, const std::string& Name, const std::function<void(float)>& Delegate, float Value) {
	m_AxisKeys.insert(std::make_pair(Key, std::make_tuple(Name, Delegate, Value)));
}

void InputClass::BindActionDelegate(unsigned short Key, InputState IS, Function Delegate) {
	m_ActionKeys.insert(std::make_pair(Key, std::make_pair(IS, Delegate)));
}

void InputClass::BindMouseActionDelegate(unsigned short Key, RECT Rect, Function Delegate, Function DelegateTwo) {
	m_MouseActions.insert(std::pair<unsigned short, std::tuple<RECT, Function, Function>>(Key, std::tuple<RECT, Function, Function>(Rect, Delegate, DelegateTwo)));
}

void InputClass::SetMousePosition(short X, short Y) {
	m_MouseX = X;
	m_MouseY = Y;
}