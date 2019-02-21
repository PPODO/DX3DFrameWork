#pragma once
#include <d3dx9.h>

class TextureClass {
private:
	TCHAR* Src;
public:
	LPDIRECT3DTEXTURE9 m_Texture;
	RECT m_ImageRect;
	D3DXVECTOR3 m_ImageCenter, m_ImagePosition;
	int m_Color;

public:
	TextureClass(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect = { -1 }) : m_ImagePosition(0.f, 0.f, 0.f), m_Color(0xffffffff) {
		D3DXIMAGE_INFO ImageInfo;
		const size_t Len = lstrlen(L"Img/") + lstrlen(FileSrc) + 1;
		Src = new TCHAR[Len];
		memset(Src, 0, Len);
		lstrcpy(Src, L"Img/");
		lstrcat(Src, FileSrc);
		D3DXCreateTextureFromFileEx(Device, Src, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, &ImageInfo, nullptr, &m_Texture);
		CustomRect.left < 0 ? m_ImageRect = { 0, 0, (LONG)ImageInfo.Width, (LONG)ImageInfo.Height } : m_ImageRect = CustomRect;
		m_ImageCenter = { (float)CustomRect.right / 2.f, (float)CustomRect.bottom / 2.f, 0.f };
	}
	~TextureClass() {
		if (m_Texture) {
			m_Texture->Release();
			m_Texture = nullptr;
		}
		if (Src) {
			delete Src;
			Src = nullptr;
		}
	}
};