#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>

class D3DXClass {
private:
	LPDIRECT3D9 m_Direct9;
	LPDIRECT3DDEVICE9 m_Device;
	LPD3DXSPRITE m_Sprite;

public:
	D3DXClass();
	~D3DXClass();

	bool Init(int, int, HWND, bool);

public:
	inline LPDIRECT3DDEVICE9 GetDevice() const { return m_Device; }
	inline LPD3DXSPRITE GetSprite() const { return m_Sprite; }
};

