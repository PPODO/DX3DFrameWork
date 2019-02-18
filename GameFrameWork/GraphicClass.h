#pragma once
#include <d3dx9.h>
#include <Windows.h>
#include <thread>
#include <mutex>
#include <iostream>

const bool bFullScreen = false;

class D3DXClass;
class ActorClass;

class TextureClass {
private:
	TCHAR* m_ImageSrc;

public:
	LPDIRECT3DTEXTURE9 m_Texture;
	RECT m_ImageRect;
	D3DXVECTOR3 m_Center, m_Position;
	int m_Color = 0xffffffff;

public:
	TextureClass() : m_Texture(nullptr), m_Center(0, 0, 0), m_Position(0, 0, 0), m_ImageSrc(nullptr) {};
	TextureClass(LPDIRECT3DDEVICE9 Device, LPCWSTR ImageSrc, RECT CustomRect = { -1 }) : m_Texture(nullptr), m_Center(0, 0, 0), m_Position(0, 0, 0), m_ImageSrc(nullptr) {
		D3DXIMAGE_INFO ImageInfo;
		int LenSize = lstrlen(L"Img/") + lstrlen(ImageSrc) + 1;
		m_ImageSrc = new TCHAR[LenSize];
		memset(m_ImageSrc, 0, LenSize);
		lstrcpy(m_ImageSrc, L"Img/");
		lstrcat(m_ImageSrc, ImageSrc);
		m_ImageSrc[LenSize - 1] = '\0';
		D3DXCreateTextureFromFileEx(Device, m_ImageSrc, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, &ImageInfo, 0, &m_Texture);
		CustomRect.left > 0 ? m_ImageRect = CustomRect : m_ImageRect = RECT{ 0, 0, (LONG)ImageInfo.Width, (LONG)ImageInfo.Height };
		m_Center = D3DXVECTOR3((((FLOAT)m_ImageRect.right) / 2.f), (((FLOAT)m_ImageRect.bottom) / 2.f), 0.f);
	};
	~TextureClass() {
		std::cout << "HI";
		if (m_Texture) {
			m_Texture->Release();
			m_Texture = nullptr;
		}
		if (m_ImageSrc) {
			delete m_ImageSrc;
			m_ImageSrc = nullptr;
		}
	}
};

class GraphicClass {
private:
	D3DXClass* m_D3DX;
	ActorClass* m_TempActorClass;

private:


public:
	GraphicClass() : m_D3DX(nullptr) {};
	~GraphicClass();

	bool Init(int, int, HWND);
	bool Render();
	void Shutdown();

public:
	void SetActorClass(ActorClass* ActorClass) { m_TempActorClass = ActorClass; }
	D3DXClass* GetD3DClass() const { return m_D3DX; }
};