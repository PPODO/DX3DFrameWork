#pragma once
#include "TextureClass.h"

enum ECollisionType { ECT_NONE, ECT_ALLBLOCK, ECT_PLAYER, ECT_OBSTACLE };

class Actor {
private:
	static RECT m_WindowSize;

private:
	TextureClass* m_Image;

protected:
	ECollisionType m_CollisionType;
	D3DXVECTOR2 m_MoveSpeed;
	bool m_bIsActivated;

public:
	Actor();
	virtual ~Actor();
	
public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = L"");
	virtual void Update(float DeltaTime);
	virtual void Render(LPD3DXSPRITE Sprite);
	virtual bool IsItOutOfScreen();
	virtual void CollisionEventBeginByOtherActor(Actor*) = 0;

public:
	void SetActorMoveSpeed(const D3DXVECTOR2& MoveSpeed) { m_MoveSpeed = MoveSpeed; }
	D3DXVECTOR2 GetActorMoveSpeed() const { return m_MoveSpeed; }
	void SetActorCollisionType(const ECollisionType& ECT) { m_CollisionType = ECT; }
	ECollisionType GetActorCollisionType() const { return m_CollisionType; }
	bool GetActorIsActivated() const { return m_bIsActivated; }
	TextureClass* GetActorImage() const { return m_Image; }
};