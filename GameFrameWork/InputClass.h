#pragma once
#include <Windows.h>
#include <functional>
#include <map>

typedef std::function<void(float)> AxisFunction;
typedef std::function<void()> ActionFunction;

enum EActionInputState { IE_Pressed, IE_Released };

#pragma pack(push, 1)
typedef struct {
	bool m_bIsPressed;
	AxisFunction m_Delegate;
}DelegateAxisInfomations;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
	EActionInputState m_InputState;
	ActionFunction m_Delegate;
}DelegateActionInfomations;
#pragma pack(pop)

class InputClass {
private:
	bool m_Keys[256];
	std::map<unsigned short, DelegateAxisInfomations> m_BindAxisKeys;
	std::map<unsigned short, DelegateActionInfomations> m_BindActionKeys;

private:
	inline void CheckBindKeys(unsigned short Key, EActionInputState InputState, bool bPressed);

public:
	InputClass();
	~InputClass();

	void Frame();
	void KeyIsDown(unsigned short Key);
	void KeyIsUp(unsigned short Key);

public:
	void BindAxisDelegate(unsigned short Key, AxisFunction Delegate);
	void BindActionDelegate(unsigned short Key, EActionInputState InputState, ActionFunction Delegate);

};

inline void InputClass::CheckBindKeys(unsigned short Key, EActionInputState InputState, bool bPressed) {
	if (m_BindActionKeys.find(Key) != m_BindActionKeys.cend()) {
		auto BindActionKey = m_BindActionKeys.find(Key);
		if (BindActionKey->second.m_InputState == InputState) {
			BindActionKey->second.m_Delegate();
		}
	}
	else {
		if (m_BindAxisKeys.find(Key) != m_BindAxisKeys.cend()) {
			m_BindAxisKeys.find(Key)->second.m_bIsPressed = bPressed;
		}
	}
}