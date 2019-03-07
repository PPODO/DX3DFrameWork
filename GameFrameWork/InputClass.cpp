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
	// Axis 키가 비어 있지 않았을 경우
	if (!m_AxisKeys.empty()) {
		for (auto AxisKey : m_AxisKeys) {
			// 현재 키가 눌러져 있는지
			if (std::get<0>(AxisKey.second)) {
				// 눌러져 있다면 함수를 실행
				std::get<1>(AxisKey.second)();
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
	// Key와 동일한 값을 가지고 있는 데이터의 영역을 구해옴.
	auto Iterator = m_MouseActions.equal_range(Key);
	// 찾았을 경우
	if (Iterator.first != m_MouseActions.cend()) {
		// 데이터 영역을 처음부터 끝까지 돌림
		for (auto It = Iterator.first; It != Iterator.second; ++It) {
			// 현재 마우스 좌표와 오버랩되는 버튼이 있는지 검사
			if (CheckPosInRect(std::get<0>(It->second), X, Y)) {
				m_MouseActionDataSave = &It->second;
				// 버튼 색상을 변경
				std::get<1>(*m_MouseActionDataSave)();
				break;
			}
		}
	}
}

void InputClass::MouseIsUp(unsigned short Key, short X, short Y) {
	// Key와 동일한 값을 가지고 있는 데이터의 영역을 구해옴.
	auto Iterator = m_MouseActions.equal_range(Key);
	// 찾았을 경우
	if (Iterator.first != m_MouseActions.cend()) {
		// 데이터 영역을 처음부터 끝까지 돌림
		for (auto It = Iterator.first; It != Iterator.second; ++It) {
			// 현재 마우스 좌표와 오버랩되는 버튼이 있는지 검사
			if (CheckPosInRect(std::get<0>(It->second), X, Y) && std::get<0>(It->second) == std::get<0>(*m_MouseActionDataSave)) {
				// 버튼이 눌러졌을 때를 정의한 함수를 호출함.
				std::get<2>(*m_MouseActionDataSave)();
				break;
			}
		}
		if (m_MouseActionDataSave) {
			// 이전에 생삭이 바뀐 버튼이 존재한다면, 색상을 원래대로 만듦
			std::get<1>(*m_MouseActionDataSave)();
		}
	}
	m_MouseActionDataSave = nullptr;
}

void InputClass::BindAxisDelegate(unsigned short Key, Function Delegate) {
	m_AxisKeys.insert(std::make_pair(Key, std::make_tuple(false, Delegate)));
}

void InputClass::BindActionDelegate(unsigned short Key, InputState IS, Function Delegate) {
	m_ActionKeys.insert(std::make_pair(Key, std::make_tuple(IS, Delegate)));
}

void InputClass::BindMouseActionDelegate(unsigned short Key, RECT Rect, Function Delegate, Function DelegateTwo) {
	m_MouseActions.insert(std::pair<unsigned short, std::tuple<RECT, Function, Function>>(Key, std::tuple<RECT, Function, Function>(Rect, Delegate, DelegateTwo)));
}

void InputClass::SetMousePosition(short X, short Y) {
	m_MouseX = X;
	m_MouseY = Y;
}