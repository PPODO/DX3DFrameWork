#include "D3DXClass.h"

D3DXClass::D3DXClass() : m_Device(nullptr), m_Direct9(nullptr) {
}

D3DXClass::~D3DXClass() {
	if (m_Device) {
		m_Device->Release();
		m_Device = nullptr;
	}
	if (m_Direct9) {
		m_Direct9->Release();
		m_Direct9 = nullptr;
	}
}

bool D3DXClass::Init(int Width, int Height, HWND hWnd, bool FullScreen) {
	if (!(m_Direct9 = Direct3DCreate9(D3D_SDK_VERSION))) {
		return false;
	}
	D3DPRESENT_PARAMETERS Params;
	memset(&Params, 0, sizeof(D3DPRESENT_PARAMETERS));
	Params.Windowed = !FullScreen;
	Params.BackBufferCount = 1;
	Params.BackBufferWidth = Width;
	Params.BackBufferHeight = Height;
	Params.BackBufferFormat = D3DFMT_UNKNOWN;
	Params.EnableAutoDepthStencil = true;
	Params.AutoDepthStencilFormat = D3DFMT_D16;
	Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	if (FullScreen) {
		Params.BackBufferFormat = D3DFMT_X8R8G8B8;
		Params.hDeviceWindow = hWnd;
	}

	D3DCAPS9 Caps;
	int nHardVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	m_Direct9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &Caps);
	if (Caps.DevCaps & D3DDEVCAPS_HWRASTERIZATION) {
		nHardVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	if (FAILED(m_Direct9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, nHardVertexProcessing | D3DCREATE_MULTITHREADED, &Params, &m_Device))) {
		return false;
	}
	if (FAILED(D3DXCreateSprite(m_Device, &m_Sprite))) {
		return false;
	}
	return true;
}