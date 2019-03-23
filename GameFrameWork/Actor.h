#pragma once
#include "TextureClass.h"
#include <functional>
#include<iostream>
#include <chrono>

const float Gravity = 0.5f;

enum ECollisionType { ECT_NONE, ECT_ALLBLOCK, ECT_PLAYER, ECT_OBSTACLE, ECT_ENEMY, ECT_PROJECTILE };

class Actor {
private:
	static RECT m_WindowSize;

private:
	TextureClass* m_Image;

	bool m_bIsTimerLoop, m_bIsUseTimer;
	std::chrono::duration<float> m_DelayTime;
	std::chrono::system_clock::time_point m_LastTime;

	inline void ProcessingTimer();

protected:
	std::string m_Name;
	ECollisionType m_CollisionType;
	D3DXVECTOR2 m_MoveSpeed;
	bool m_bIsActivated;

protected:
	inline virtual void TimerNotification();

public:
	Actor();
	virtual ~Actor();
	
public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = L"");
	virtual void Update(float DeltaTime);
	virtual void Render(LPD3DXSPRITE Sprite);
	virtual bool IsItOutOfScreen();
	virtual void SpawnActor(const D3DXVECTOR3& = { 0.f,0.f,0.f });
	virtual void CollisionEventBeginByOtherActor(Actor*) = 0;

public:
	void SetTimer(float DelayTime, bool IsLoop = false) { m_bIsUseTimer = true; m_bIsTimerLoop = IsLoop; m_DelayTime = std::chrono::duration<float>(DelayTime); m_LastTime = std::chrono::system_clock::now(); };
	void SetActorIsActivated(bool b) { m_bIsActivated = b; }
	void SetActorMoveSpeed(const D3DXVECTOR2& MoveSpeed) { m_MoveSpeed = MoveSpeed; }
	D3DXVECTOR2 GetActorMoveSpeed() const { return m_MoveSpeed; }
	void SetActorCollisionType(const ECollisionType& ECT) { m_CollisionType = ECT; }
	ECollisionType GetActorCollisionType() const { return m_CollisionType; }
	bool GetActorIsActivated() const { return m_bIsActivated; }
	TextureClass* GetActorImage() const { return m_Image; }
	RECT GetWindowSize() const { return m_WindowSize; }
	inline std::string GetName() const { return m_Name; }

};

inline void Actor::ProcessingTimer() {
	if (std::chrono::system_clock::now() - m_LastTime > m_DelayTime) {
		if (!m_bIsTimerLoop) {
			m_bIsUseTimer = false;
			m_LastTime = std::chrono::system_clock::now();
		}
		TimerNotification();
	}
}

inline void Actor::TimerNotification() {
	
}