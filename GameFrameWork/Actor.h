#pragma once
#include <d3dx9.h>
#include <vector>
#include "EventClass.h"

enum CollisionType { CT_PLAYER, CT_ENEMY, CT_PROJECTILE };

class Actor {
private:
	static RECT m_WindowSize;

protected:
	class TextureClass* m_Texture;
	CollisionType m_Collision;
	bool m_bIsActive;

protected:
	virtual void OutOfScreen();

public:
	Actor();

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 });
	virtual void Update(float DeltaTime);
	virtual void Render(LPD3DXSPRITE Sprite);
	virtual void Destroy();
	virtual void TriggerCollisionEventByOtherActor(Actor*);
	virtual bool IsItOutOfScreen();

public:
	void SetActivation(bool b) { m_bIsActive = b; }
	void SetCollisionType(CollisionType CT) { m_Collision = CT; }
	
	inline CollisionType GetActorCollisionType() const { return m_Collision; }
	inline bool GetIsActivation() const { return m_bIsActive; }
	class TextureClass* GetTexture() const { return m_Texture; }
	inline RECT GetWindowSize() const { return m_WindowSize; }

};