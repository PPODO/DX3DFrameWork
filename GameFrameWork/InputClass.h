#pragma once
#include <functional>
#include <tuple>
#include <map>
#include <Windows.h>

enum InputState { IE_None, IE_Pressed, IE_Released };

typedef std::function<void()> Function;

class InputClass {
private:
	std::map<unsigned short, std::tuple<bool, Function>> m_AxisKeys;
	std::map<unsigned short, std::tuple<InputState, Function>> m_ActionKeys;
	std::map<unsigned short, std::tuple<RECT, Function, Function>> m_MouseActions;
	std::tuple<RECT, Function, Function> m_MouseActionDataSave;

	short m_MouseX, m_MouseY;

public:
	InputClass();
	~InputClass();

public:
	void Frame();

	void KeyIsDown(unsigned short);
	void KeyIsUp(unsigned short);
	void MouseIsDown(unsigned short, short, short);
	void MouseIsUp(unsigned short, short, short);

	void BindAxisDelegate(unsigned short, Function);
	void BindActionDelegate(unsigned short, Function);
	void BindMouseActionDelegate(unsigned short, RECT, Function, Function);

	void SetMousePosition(short, short);

	inline void CheckBindKeys(unsigned short, InputState, bool);
	inline bool CheckPosInRect(RECT, short, short);
};

inline void InputClass::CheckBindKeys(unsigned short Key, InputState IS, bool bPressed) {
	if (m_ActionKeys.find(Key) != m_ActionKeys.cend()) {
		auto ActionKey = m_ActionKeys.find(Key);
		if (std::get<0>(ActionKey->second) == IS) {
			std::get<1>(ActionKey->second)();
		}
	}
	else {
		if (m_AxisKeys.find(Key) != m_AxisKeys.cend()) {
			std::get<0>(m_AxisKeys.find(Key)->second) = bPressed;
		}
	}
}

inline bool InputClass::CheckPosInRect(RECT Rect, short X, short Y) {
	if (Rect.left <= X && Rect.right >= X && Rect.bottom >= Y && Rect.top <= Y) {
		return true;
	}
	return false;
}