#include "SystemClass.h"
#include "MessageQueueClass.h"
#include "GraphicClass.h"
#include "D3DXClass.h"
#include "ActorClass.h"
#include "InputClass.h"
#include <iostream>

SystemClass* SystemClass::m_Application = nullptr;

SystemClass::SystemClass() : DeltaTime(0.f), m_ActorManager(nullptr), m_D3DX(nullptr), m_hInstance(nullptr), m_hWnd(nullptr), WaitForRender(true) {
	MessageQueueClass::GetInst();
	GraphicClass::GetInst();
}

SystemClass::~SystemClass() {
	MessageQueueClass::DestroySingleton();
}

bool SystemClass::InitWindow(int& Width, int& Height) {
	m_hInstance = GetModuleHandle(nullptr);
	m_ApplicationName = L"D3DXFameWork";

	WNDCLASSEX WndClass;
	WndClass.cbSize = sizeof(WNDCLASSEX);
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	WndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	WndClass.hIconSm = WndClass.hIcon;
	WndClass.hInstance = m_hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = m_ApplicationName;
	WndClass.lpszMenuName = nullptr;
	WndClass.style = CS_VREDRAW | CS_HREDRAW;
	RegisterClassEx(&WndClass);

	Width = GetSystemMetrics(SM_CXSCREEN);
	Height = GetSystemMetrics(SM_CYSCREEN);

	if (0) {
		DEVMODE DevMode;
		ZeroMemory(&DevMode, sizeof(DEVMODE));
		DevMode.dmSize = sizeof(DevMode);
		DevMode.dmPelsWidth = Width;
		DevMode.dmPelsHeight = Height;
		DevMode.dmBitsPerPel = 32;
		DevMode.dmFields = DM_BITSPERPEL | DM_PELSHEIGHT | DM_PELSWIDTH;
		ChangeDisplaySettings(&DevMode, CDS_FULLSCREEN);
	}
	else {
		Width = 800;
		Height = 600;
	}

	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_ApplicationName, m_ApplicationName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, Width, Height, nullptr, (HMENU)nullptr, m_hInstance, nullptr);
	ShowWindow(m_hWnd, SW_SHOW);
	return true;
}

bool SystemClass::Init() {
	int Width, Height;
	if (!InitWindow(Width, Height)) {
		return false;
	}

	m_D3DX = new D3DXClass;
	if (!m_D3DX || !m_D3DX->Init(Width, Height, m_hWnd)) {
		return false;
	}
	m_Input = new InputClass;
	if (!m_Input) {
		return false;
	}
	MessageQueueClass::GetInst()->PushMessage(MS_INIT, std::bind(&GraphicClass::Init, GraphicClass::GetInst(), m_D3DX->GetDevice(), m_D3DX->GetSprite()));
//	MessageQueueClass::GetInst()->PushMessage(MS_RENDER, std::bind(&GraphicClass::Render, GraphicClass::GetInst(), m_D3DX->GetDevice() ));

	m_ActorManager = new ActorClass;
	if (!m_ActorManager || !m_ActorManager->Init(m_D3DX->GetDevice())) {
		return false;
	}
	return true;
}

bool SystemClass::Frame(float DeltaTime) {
	WaitForRender = true;
	MessageQueueClass::GetInst()->PushMessage(MS_RENDER, std::bind(&GraphicClass::Render, GraphicClass::GetInst(), m_ActorManager->GetCurrentActor()));

	m_ActorManager->Frame(DeltaTime);
	m_Input->Frame();

	while (WaitForRender);
	return true;
}

void SystemClass::Run() {
	MSG Message;
	ZeroMemory(&Message, sizeof(MSG));

	while (Message.message != WM_QUIT) {
		if (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else {
			if (Timer.UpdateFPS(DeltaTime)) {
				if (!Frame(DeltaTime)) {
					return;
				}
			}
		}
	}
}

void SystemClass::ShutdownWindow() {
	if (0) {
		ChangeDisplaySettings(nullptr, 0);
	}
	DestroyWindow(m_hWnd);
	m_hWnd = nullptr;

	UnregisterClass(m_ApplicationName, m_hInstance);
	m_Application = nullptr;
}

void SystemClass::Shutdown() {
	if (m_ActorManager) {
		delete m_ActorManager;
		m_ActorManager = nullptr;
	}
	if (m_Input) {
		delete m_Input;
		m_Input = nullptr;
	}
	if (m_D3DX) {
		delete m_D3DX;
		m_D3DX = nullptr;
	}
	MessageQueueClass::GetInst()->PushMessage(MS_DESTROY, []() {});

	ShutdownWindow();
	DestroySingleton();
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
		// 해상도 기준이므로 수정ㅂㄹ
	case WM_LBUTTONDOWN:
		m_Input->MouseIsDown(m_ActorManager->GetCurrentStage(), LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_LBUTTONUP:
		m_Input->MouseIsUp(m_ActorManager->GetCurrentStage(), LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_MOUSEMOVE:
		m_Input->SetMousePosition(LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_KEYDOWN:
		m_Input->KeyIsDown(wParam);
		return 0;
	case WM_KEYUP:
		m_Input->KeyIsUp(wParam);
		return 0;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return SystemClass::GetInst()->MessageHandler(hWnd, iMessage, wParam, lParam);
}