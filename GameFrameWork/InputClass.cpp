#include "InputClass.h"

InputClass::InputClass() {
}

InputClass::~InputClass() {
	m_ActionKeys.clear();
	m_AxisKeys.clear();
}

void InputClass::Frame() {
	if (!m_AxisKeys.empty()) {
		for (auto AxisKey : m_AxisKeys) {
			if (std::get<0>(AxisKey.second)) {
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

void InputClass::BindAxisDelegate(unsigned short Key, std::function<void(float)> Delegate) {
	m_AxisKeys.insert(std::make_pair(Key, std::make_tuple(false, Delegate)));
}

void InputClass::BindActionDelegate(unsigned short Key, std::function<void()> Delegate) {
	m_ActionKeys.insert(std::make_pair(Key, std::make_tuple(IE_None, Delegate)));
}