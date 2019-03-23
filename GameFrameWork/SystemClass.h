#pragma once
#include <Windows.h>
#include "Singleton.h"
#include "TimerClass.h"

class SystemClass : public Singleton<SystemClass> {
private:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	LPCTSTR m_ApplicationName;
	RECT m_WindowSize;
	bool m_bIsFullScreen;

	class D3DXClass* m_D3DX;
	class ActorClass* m_ActorManager;
	class InputClass* m_Input;

	TimerClass Timer;
	float DeltaTime;
	bool WaitForRender;

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

public:
	inline void SetWaitForRender(bool b) { WaitForRender = b; }
	inline HWND GetWindowHandle() const { return m_hWnd; }
	inline class InputClass* GetInputManager() const { return m_Input; }
	inline class ActorClass* GetActorManager() const { return m_ActorManager; }
	inline RECT GetWindowSize() const { return m_WindowSize; };
};

static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);