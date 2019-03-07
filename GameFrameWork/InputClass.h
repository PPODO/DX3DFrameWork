#pragma once
#include <functional>
#include <tuple>
#include <map>
#include <utility>
#include <Windows.h>

enum InputState { IE_None, IE_Pressed, IE_Released };

typedef std::function<void()> Function;

class InputClass {
private:
	// Axis축, 지속적인 입력이 필요할 때 사용
	std::map<unsigned short, std::tuple<bool, Function>> m_AxisKeys;
	// Action, 키 상태에 따른 이벤트
	std::map<unsigned short, std::tuple<InputState, Function>> m_ActionKeys;
	// 마우스 클릭(버튼을 위한 입력), Rect는 검사 영역, 첫 번째 Function은 버튼을 눌렀을 때 버튼의 색상을 변경함, 두 번째 Function은 버튼을 클릭 했을 경우 실행될 함수
	std::multimap<unsigned short, std::tuple<RECT, Function, Function>> m_MouseActions;
	std::tuple<RECT, Function, Function>* m_MouseActionDataSave;

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
	void BindActionDelegate(unsigned short, InputState, Function);
	void BindMouseActionDelegate(unsigned short, RECT, Function, Function);

	void SetMousePosition(short, short);

	inline void CheckBindKeys(unsigned short, InputState, bool);
	inline bool CheckPosInRect(RECT, short, short);
};

inline void InputClass::CheckBindKeys(unsigned short Key, InputState IS, bool bPressed) {
	// 누른 키가 Action에 바인딩 되어 있을 경우
	if (m_ActionKeys.find(Key) != m_ActionKeys.cend()) {
		auto ActionKey = m_ActionKeys.find(Key);
		if (std::get<0>(ActionKey->second) == IS) {
			std::get<1>(ActionKey->second)();
		}
	}
	else {
		// 누른 키가 Axis에 바인딩 되어 있을 경우
		if (m_AxisKeys.find(Key) != m_AxisKeys.cend()) {
			// 상태 값 변경
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