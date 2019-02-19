#pragma once
#include <Windows.h>

class GraphicClass;
class InputClass;
class ActorClass;

class SystemClass {
private:
	GraphicClass* m_Graphic;
	InputClass* m_Input;
	ActorClass* m_Actor;
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	LPCWSTR m_ApplicationName;

private:
	bool InitWindow(int&, int&);
	bool Frame();
	void ShutdownWindow();

public:
	static SystemClass* m_Application;
	static inline SystemClass* GetInst() { if (!m_Application) { m_Application = new SystemClass; }  return m_Application; }

public:
	SystemClass() : m_Graphic(nullptr), m_Input(nullptr), m_hWnd(nullptr), m_hInstance(nullptr), m_ApplicationName(nullptr) {};
	~SystemClass();

	bool Init();
	void Run();
	void Shutdown();

public:
	LRESULT CALLBACK MessageHandler(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	inline InputClass* GetInputClass() const { return m_Input; }
	inline HWND GetWindowHandle() const { return m_hWnd; }

};

static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);