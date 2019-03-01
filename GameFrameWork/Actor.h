#pragma once
#include <d3dx9.h>
#include <vector>

class Actor {
protected:
	class TextureClass* m_Texture;
	bool m_bIsActive;

public:
	Actor();

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 });
	virtual void Update(float DeltaTime);
	virtual void Render(LPD3DXSPRITE Sprite);
	virtual void Destroy();

public:
	void SetActivation(bool b) { m_bIsActive = b; }
	bool GetIsActivation() const { return m_bIsActive; }

};