#pragma once
#include <Windows.h>
#include <functional>
#include <map>

typedef std::function<void(float)> Function;

#pragma pack(push, 1)
typedef struct {
	bool m_bIsPressed;
	Function m_Delegate;
}DelegateInfomations;
#pragma pack(pop)

class InputClass {
private:
	bool m_Keys[256];
	std::map<unsigned short, DelegateInfomations> m_BindKeys;

public:
	InputClass();
	~InputClass();

	void Frame();
	void KeyIsDown(unsigned short Key);
	void KeyIsUp(unsigned short Key);

public:
	void BindDelegate(unsigned short Key, Function Delegate);

};