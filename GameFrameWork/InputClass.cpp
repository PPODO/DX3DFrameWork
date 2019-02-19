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
	CheckBindKeys(Key, IE_Pressed, true);
}

void InputClass::KeyIsUp(unsigned short Key) {
	m_Keys[Key] = false;
	CheckBindKeys(Key, IE_Released, false);
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