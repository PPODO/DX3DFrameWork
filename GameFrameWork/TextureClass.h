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
	TextureClass(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect = { -1 }) : m_ImagePosition(0.f, 0.f, 0.f), m_Color(0xffffffff), m_Texture(nullptr) {
		D3DXIMAGE_INFO ImageInfo;
		const size_t Len = lstrlen(L"Img/") + lstrlen(FileSrc) + 1;
		Src = new TCHAR[Len];
		memset(Src, 0, Len);
		lstrcpy(Src, L"Img/");
		lstrcat(Src, FileSrc);
		D3DXCreateTextureFromFileEx(Device, Src, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, &ImageInfo, nullptr, &m_Texture);
		CustomRect.left < 0 ? m_ImageRect = { 0, 0, (LONG)ImageInfo.Width, (LONG)ImageInfo.Height } : m_ImageRect = CustomRect;
		m_ImageCenter = { (float)m_ImageRect.right / 2.f, (float)m_ImageRect.bottom / 2.f, 0.f };
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

	inline void Render(LPD3DXSPRITE Sprite) {
		if (Sprite) {
			Sprite->Draw(m_Texture, &m_ImageRect, &m_ImageCenter, &m_ImagePosition, m_Color);
		}
	}

	inline void SetImageCenter(const D3DXVECTOR3& Vector) { m_ImageCenter = Vector; }
	inline void SetPosition(const D3DXVECTOR3& Vector) { m_ImagePosition = Vector; }
	inline void AddPosition(const D3DXVECTOR3& Vector) { m_ImagePosition += Vector; }
	inline void AddXPosition(const float Value) { m_ImagePosition.x += Value; }
	inline void AddYPosition(const float Value) { m_ImagePosition.y += Value; }

	inline D3DXVECTOR3 GetImageCenter() const { return m_ImageCenter; }
	inline D3DXVECTOR3 GetPosition() const { return m_ImagePosition; }
	inline RECT GetRect() const { return m_ImageRect; }
};