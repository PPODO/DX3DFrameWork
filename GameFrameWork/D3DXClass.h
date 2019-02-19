#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class TextureClass;

class D3DXClass {
private:
	LPDIRECT3D9 m_Direct9;
	LPDIRECT3DDEVICE9 m_DirectDevice;
	LPD3DXSPRITE m_DirectSprite;

public:
	D3DXClass() : m_Direct9(nullptr), m_DirectDevice(nullptr), m_DirectSprite(nullptr) {};
	~D3DXClass();

	bool Init(int, int, HWND);

public:
	inline bool BeginScene();
	inline void EndScene();

	inline LPDIRECT3DDEVICE9 GetDevice() const { return m_DirectDevice; }
	inline LPD3DXSPRITE GetSprite() const { return m_DirectSprite; }
};

inline bool D3DXClass::BeginScene() {
	m_DirectDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.f, 0);
	if (SUCCEEDED(m_DirectDevice->BeginScene())) {
		return SUCCEEDED(m_DirectSprite->Begin(D3DXSPRITE_ALPHABLEND));
	}
	return false;
}

inline void D3DXClass::EndScene() {
	m_DirectSprite->End();
	m_DirectDevice->EndScene();
	m_DirectDevice->Present(nullptr, nullptr, nullptr, nullptr);
}