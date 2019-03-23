#pragma once
#include <functional>
#include <tuple>
#include <map>
#include <string>
#include <utility>
#include <Windows.h>

#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44
#define VK_W 0x57

enum InputState { IE_None, IE_Pressed, IE_Released };

typedef std::function<void()> Function;

class InputClass {
private:
	std::map<unsigned short, std::tuple<std::string, std::function<void(float)>, float>> m_AxisKeys;
	std::multimap<unsigned short, std::pair<InputState, Function>> m_ActionKeys;

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

	void BindAxisDelegate(unsigned short, const std::string& Name, const std::function<void(float)>&, float);
	void BindActionDelegate(unsigned short, InputState, Function);
	void BindMouseActionDelegate(unsigned short, RECT, Function, Function);

	void SetMousePosition(short, short);

	inline void CheckBindKeys(unsigned short, InputState, bool);
	inline bool CheckPosInRect(RECT, short, short);
};

inline void InputClass::CheckBindKeys(unsigned short Key, InputState IS, bool bPressed) {
	auto ActionKeys = m_ActionKeys.equal_range(Key);
	if (ActionKeys.first != m_ActionKeys.cend()) {
		for (auto It = ActionKeys.first; It != ActionKeys.second; ++It) {
			if (std::get<0>(It->second) == IS) {
				std::get<1>(It->second)();
			}
		}
	}
}

inline bool InputClass::CheckPosInRect(RECT Rect, short X, short Y) {
	if (Rect.left <= X && Rect.right >= X && Rect.bottom >= Y && Rect.top <= Y) {
		return true;
	}
	return false;
}