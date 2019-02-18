#include "InputClass.h"

InputClass::InputClass() {
	memset(m_Keys, 0, sizeof(m_Keys));

}

InputClass::~InputClass() {

}

void InputClass::Frame() {
	if (!m_BindKeys.empty()) {
		for (auto Iterator = m_BindKeys.cbegin(); Iterator != m_BindKeys.cend(); ++Iterator) {
			if (Iterator->second.m_bIsPressed) {
				Iterator->second.m_Delegate(Iterator->second.m_Value);
			}
		}
	}
}

void InputClass::KeyIsDown(unsigned short Key) {
	m_Keys[Key] = true;
	if (m_BindKeys.find(Key) != m_BindKeys.cend()) {
		m_BindKeys.find(Key)->second.m_bIsPressed = true;
	}
}

void InputClass::KeyIsUp(unsigned short Key) {
	m_Keys[Key] = false;
	if (m_BindKeys.find(Key) != m_BindKeys.cend()) {
		m_BindKeys.find(Key)->second.m_bIsPressed = false;
	}
}

void InputClass::BindDelegate(unsigned short Key, Function Delegate, float Value) {
	DelegateInfomations DI;
	memset(&DI, 0, sizeof(DI));
	DI.m_Delegate = Delegate;
	m_BindKeys.insert(std::make_pair(Key, DI));
}