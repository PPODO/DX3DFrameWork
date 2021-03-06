#include "SystemClass.h"
#include "D3DXClass.h"
#include "ActorClass.h"
#include "InputClass.h"
#include "MessageQueueClass.h"
#include "GraphicClass.h"
#include "EventClass.h"

SystemClass* SystemClass::m_Application;

SystemClass::SystemClass() : m_bIsFullScreen(false), DeltaTime(0.f), m_ActorManager(nullptr), m_D3DX(nullptr), m_hInstance(nullptr), m_hWnd(nullptr), WaitForRender(true), m_Input(nullptr) {
	MessageQueueClass::GetInst();
	GraphicClass::GetInst();
	EventClass::GetInst();
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

	Width = 1600;
	Height = 900;

	RECT Size = { 0, 0, Width, Height };
	AdjustWindowRectEx(&Size, WS_OVERLAPPEDWINDOW, false, WS_EX_APPWINDOW);
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_ApplicationName, m_ApplicationName, (!m_bIsFullScreen ? WS_OVERLAPPEDWINDOW : WS_EX_TOPMOST | WS_POPUP), CW_USEDEFAULT, CW_USEDEFAULT, Size.right - Size.left, Size.bottom - Size.top, nullptr, (HMENU)nullptr, m_hInstance, nullptr);
	ShowWindow(m_hWnd, SW_SHOW);
	return true;
}

bool SystemClass::Init() {
	int Width, Height;
	if (!InitWindow(Width, Height)) {
		return false;
	}
	GetClientRect(m_hWnd, &m_WindowSize);

	m_D3DX = new D3DXClass;
	if (!m_D3DX || !m_D3DX->Init(Width, Height, m_hWnd, m_bIsFullScreen)) {
		return false;
	}
	
	m_Input = new InputClass;
	if (!m_Input) {
		return false;
	}

	MessageQueueClass::GetInst()->PushMessage(MS_INIT, std::bind(&GraphicClass::Init, GraphicClass::GetInst(), m_D3DX->GetDevice(), m_D3DX->GetSprite()));
	
	m_ActorManager = new ActorClass;
	if (!m_ActorManager || !m_ActorManager->Init(m_D3DX->GetDevice())) {
		return false;
	}

	return true;
}

bool SystemClass::Frame(float DeltaTime) {
	WaitForRender = true;
	m_ActorManager->Frame(DeltaTime);
	if (m_ActorManager->GetCurrentStage() > 0) {
		m_Input->Frame();
	}
	
	MessageQueueClass::GetInst()->PushMessage(MS_RENDER, std::bind(&GraphicClass::Render, GraphicClass::GetInst(), [this]() { m_ActorManager->Render(m_D3DX->GetSprite()); }));
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
	DestroyWindow(m_hWnd);
	m_hWnd = nullptr;

	UnregisterClass(m_ApplicationName, m_hInstance);
	m_Application = nullptr;
}

void SystemClass::Shutdown() {
	EventClass::DestroySingleton();
	MessageQueueClass::GetInst()->PushMessage(MS_DESTROY, []() {});

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

	ShutdownWindow();
	DestroySingleton();
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
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