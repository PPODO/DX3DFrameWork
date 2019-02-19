#include "InputClass.h"

InputClass::InputClass() {
	memset(m_Keys, 0, sizeof(m_Keys));

}

InputClass::~InputClass() {

}

void InputClass::Frame() {
	if (!m_BindAxisKeys.empty()) {
		for (auto Iterator = m_BindAxisKeys.cbegin(); Iterator != m_BindAxisKeys.cend(); ++Iterator) {
			if (Iterator->second.m_bIsPressed) {
				Iterator->second.m_Delegate(0);
			}
		}
	}
}

void InputClass::KeyIsDown(unsigned short Key) {
	m_Keys[Key] = true;
	if (m_BindActionKeys.find(Key) != m_BindActionKeys.cend()) {
		auto BindActionKey = m_BindActionKeys.find(Key);
		if (BindActionKey->second.m_InputState == IE_Pressed) {
			BindActionKey->second.m_Delegate();
		}
	}
	else {
		if (m_BindAxisKeys.find(Key) != m_BindAxisKeys.cend()) {
			m_BindAxisKeys.find(Key)->second.m_bIsPressed = true;
		}
	}
}

void InputClass::KeyIsUp(unsigned short Key) {
	m_Keys[Key] = false;
	if (m_BindActionKeys.find(Key) != m_BindActionKeys.cend()) {
		auto BindActionKey = m_BindActionKeys.find(Key);
		if (BindActionKey->second.m_InputState == IE_Released) {
			BindActionKey->second.m_Delegate();
		}
	}
	else {
		if (m_BindAxisKeys.find(Key) != m_BindAxisKeys.cend()) {
			m_BindAxisKeys.find(Key)->second.m_bIsPressed = false;
		}
	}
}

void InputClass::BindAxisDelegate(unsigned short Key, AxisFunction Delegate) {
	DelegateAxisInfomations DI;
	memset(&DI, 0, sizeof(DI));
	DI.m_Delegate = Delegate;
	m_BindAxisKeys.insert(std::make_pair(Key, DI));
}

void InputClass::BindActionDelegate(unsigned short Key, EActionInputState InputState, ActionFunction Delegate) {
	DelegateActionInfomations DI;
	memset(&DI, 0, sizeof(DI));
	DI.m_InputState = InputState;
	DI.m_Delegate = Delegate;
	m_BindActionKeys.insert(std::make_pair(Key, DI));
}