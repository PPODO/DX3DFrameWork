#include "SystemClass.h"
#include "D3DXClass.h"
#include "ActorClass.h"
#include "InputClass.h"
#include "MessageQueueClass.h"
#include "GraphicClass.h"
#include "EventClass.h"
#include "IdleScreenClass.h"

SystemClass* SystemClass::m_Application;

SystemClass::SystemClass() : DeltaTime(0.f), m_ActorManager(nullptr), m_D3DX(nullptr), m_hInstance(nullptr), m_hWnd(nullptr), WaitForRender(true), m_Input(nullptr) {
	MessageQueueClass::GetInst();
	GraphicClass::GetInst();
	EventClass::GetInst();
	IdleScreenClass::GetInst();
}

SystemClass::~SystemClass() {
	IdleScreenClass::DestroySingleton();
	EventClass::DestroySingleton();
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

	RECT Size = { 0, 0, Width, Height };
	AdjustWindowRectEx(&Size, WS_OVERLAPPEDWINDOW, false, WS_EX_APPWINDOW);
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_ApplicationName, m_ApplicationName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, Size.right - Size.left, Size.bottom - Size.top, nullptr, (HMENU)nullptr, m_hInstance, nullptr);
	ShowWindow(m_hWnd, SW_SHOW);
	return true;
}

bool SystemClass::Init() {
	int Width, Height;
	if (!InitWindow(Width, Height)) {
		return false;
	}
	GetClientRect(m_hWnd, &m_WindowSize);

	// DX 객체 생성
	m_D3DX = new D3DXClass;
	if (!m_D3DX || !m_D3DX->Init(Width, Height, m_hWnd)) {
		return false;
	}
	IdleScreenClass::GetInst()->Init(m_D3DX->GetDevice(), m_D3DX->GetSprite());
	
	// 입력 객체 생성
	m_Input = new InputClass;
	if (!m_Input) {
		return false;
	}

	// 렌더링 스레드는 따로 스레드가 돌아가는데, 아래와 같이 메시지 큐로 수행하고 싶은 작업과 그 작업을 수행할 함수를 넘겨주면 됨.
	// PushMessag(수행할 작업 열거형, 작업을 수행할 함수(인수가 std::function<void()>이기때문에 std::bind를 사용해서 함수를 넘겨줌.
	// 구글에 std::function관련해서 검색하면 이해하기 더 수월할 듯
	MessageQueueClass::GetInst()->PushMessage(MS_INIT, std::bind(&GraphicClass::Init, GraphicClass::GetInst(), m_D3DX->GetDevice(), m_D3DX->GetSprite()));

	// 로딩화면 시작
	IdleScreenClass::GetInst()->BeginDrawImage(ISS_LOADING);
	
	// 여기에서의 액터는 화면에 출력되는 모든 것을 뜻함, 리소스 로딩이 시간이 오래걸리므로 가장 마지막에 초기화를 해주는 것,
	// 그렇기에 위에서 m_LoadingManager->BeginDrawImage()이 호출된 것. 이렇게 하면 ActorManager에서 리소스를 불러오는 동안, 로딩화면이 보여지게됨
	m_ActorManager = new ActorClass;
	if (!m_ActorManager || !m_ActorManager->Init(m_D3DX->GetDevice())) {
		return false;
	}
	// 로딩화면 끝
	IdleScreenClass::GetInst()->ClearImage();
	return true;
}

bool SystemClass::Frame(float DeltaTime) {
	if (IdleScreenClass::GetInst()->GetWaitForSignal()) {
		WaitForRender = true;
		m_ActorManager->Frame(DeltaTime);
		if (m_ActorManager->GetCurrentStage() > 0) {
			m_Input->Frame();
		}

		MessageQueueClass::GetInst()->PushMessage(MS_RENDER, std::bind(&GraphicClass::Render, GraphicClass::GetInst(), [this]() { m_ActorManager->Render(m_D3DX->GetSprite()); }));
		while (WaitForRender);
	}
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
		// 전체화면이였을 경우 디스플레이 설정을 초기화시킴.
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
	// 윈도우를 파괴하는 메시지들만 WndProc에서 처리하고 나머지 키보드 입력, 마우스 입력등은 MessageHandler로 보내줌
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