#pragma once
#include "Actor.h"

class BackGroundClass : public Actor {
private:
	class BackGroundClass* m_OtherActor;
	D3DXVECTOR2 m_AdditionalSpeed, m_DefaultMoveSpeed;

private:
	void CalculateMoveSpeed();
	void BackGroundMovementProcessing();

public:
	BackGroundClass();
	virtual ~BackGroundClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc) override;
	virtual void Update(float DeltaTime, float ActorHeight) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void CollisionEventBeginByOtherActor(Actor*) override;
	virtual void ClearObject() override;

public:
	void SetOtherBackGroundActor(BackGroundClass* OtherBG) { m_OtherActor = OtherBG; }
	void SetAdditionalSpeed(const D3DXVECTOR2& Speed) { m_AdditionalSpeed = Speed * sqrt(pow(m_DefaultMoveSpeed.x, 2) + pow(m_DefaultMoveSpeed.y, 2)); }

};