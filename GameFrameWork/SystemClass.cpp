#include "SystemClass.h"
#include "GraphicClass.h"
#include "ActorClass.h"
#include "InputClass.h"
#include "D3DXClass.h"

SystemClass* SystemClass::m_Application = nullptr;

SystemClass::~SystemClass() {
}

bool SystemClass::Init() {
	int Width, Height;
	if (!InitWindow(Width, Height)) {
		return false;
	}

	m_Graphic = new GraphicClass;
	if (!m_Graphic || !m_Graphic->Init(Width, Height, m_hWnd)) {
		return false;
	}
	m_Input = new InputClass;
	if (!m_Input) {
		return false;
	}
	m_Actor = new ActorClass;
	if (!m_Actor || !m_Actor->Init(m_Graphic->GetD3DClass()->GetDevice())) {
		return false;
	}
	m_Graphic->SetActorClass(m_Actor);
	return true;
}

bool SystemClass::InitWindow(int& ScreenWidth, int& ScreenHeight) {
	m_hInstance = GetModuleHandle(nullptr);
	m_ApplicationName = L"GameFrameWork";

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

	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (bFullScreen) {
		DEVMODE DevMode;
		memset(&DevMode, 0, sizeof(DEVMODE));
		DevMode.dmSize = sizeof(DEVMODE);
		DevMode.dmPelsHeight = ScreenHeight;
		DevMode.dmPelsWidth = ScreenWidth;
		DevMode.dmBitsPerPel = 32;
		DevMode.dmFields = DM_BITSPERPEL | DM_PELSHEIGHT | DM_PELSWIDTH;
		ChangeDisplaySettings(&DevMode, CDS_FULLSCREEN);
	}
	else {
		ScreenWidth = 800;
		ScreenHeight = 600;
	}

	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_ApplicationName, m_ApplicationName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, ScreenWidth, ScreenHeight, nullptr, (HMENU)nullptr, m_hInstance, nullptr);
	ShowWindow(m_hWnd, SW_SHOW);
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
			if (!Frame()) {
				break;
			}
		}
	}
}

bool SystemClass::Frame() {
	m_Input->Frame();
	m_Actor->Frame();

	if (!m_Graphic->Render()) {
		return false;
	}
	return true;
}

void SystemClass::ShutdownWindow() {
	if (bFullScreen) {
		ChangeDisplaySettings(nullptr, 0);
	}
	DestroyWindow(m_hWnd);
	m_hWnd = nullptr;

	UnregisterClass(m_ApplicationName, m_hInstance);
	m_hInstance = nullptr;

	if (m_Application) {
		delete m_Application;
		m_Application = nullptr;
	}
}

void SystemClass::Shutdown() {
	if (m_Input) {
		delete m_Input;
		m_Input = nullptr;
	}
	if (m_Actor) {
		m_Actor->Shutdown();
		delete m_Actor;
		m_Actor = nullptr;
	}
	if (m_Graphic) {
		m_Graphic->Shutdown();
		delete m_Graphic;
		m_Graphic = nullptr;
	}
	ShutdownWindow();
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
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
	return SystemClass::GetInst() != nullptr ? SystemClass::GetInst()->MessageHandler(hWnd, iMessage, wParam, lParam) : -1;
}