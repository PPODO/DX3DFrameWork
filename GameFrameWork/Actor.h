#pragma once
#include <d3dx9.h>
#include <vector>

class Actor {
private:
	class TextureClass* m_Texture;

public:
	Actor(std::vector<Actor*>&);

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 });
	virtual void Update(float DeltaTime) = 0;
	void Render(LPD3DXSPRITE Sprite);
	virtual void Destroy() = 0;

};