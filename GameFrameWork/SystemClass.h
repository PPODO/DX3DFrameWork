#pragma once
#include <Windows.h>
#include "Singleton.h"
#include "TimerClass.h"

class SystemClass : public Singleton<SystemClass> {
private:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	LPCTSTR m_ApplicationName;

	class D3DXClass* m_D3DX;
	class ActorClass* m_ActorManager;
	class InputClass* m_Input;

	TimerClass Timer;
	float DeltaTime;

private:
	bool InitWindow(int&, int&);
	void ShutdownWindow();

public:
	SystemClass();
	~SystemClass();

	bool Init();
	bool Frame(float DeltaTime);
	void Run();
	void Shutdown();

	LRESULT CALLBACK MessageHandler(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	bool WaitForRender = true;
public:
	HWND GetWindowHandle() const { return m_hWnd; }
	class InputClass* GetInputManager() const { return m_Input; }
};

static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);