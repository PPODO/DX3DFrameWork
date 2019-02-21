#pragma once
#include <functional>
#include <tuple>
#include <map>

enum InputState { IE_None, IE_Pressed, IE_Released };

class InputClass {
private:
	std::map<unsigned short, std::tuple<bool, std::function<void(float)>>> m_AxisKeys;
	std::map<unsigned short, std::tuple<InputState, std::function<void()>>> m_ActionKeys;

public:
	InputClass();
	~InputClass();

public:
	void Frame();

	void KeyIsDown(unsigned short);
	void KeyIsUp(unsigned short);

	void BindAxisDelegate(unsigned short, std::function<void(float)>);
	void BindActionDelegate(unsigned short, std::function<void()>);

	inline void CheckBindKeys(unsigned short, InputState, bool);
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