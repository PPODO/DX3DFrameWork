#pragma once
#include "PawnClass.h"

#define FLOATNearlyEqual 2.5f

enum EPlayerType { EPT_DEFAULT };

class PlayerClass : public PawnClass {
private:
	void Jump();
	void MoveRight(float Value);

protected:
	virtual void SetupPlayerInput() override;
	virtual void PlayStartMoveToLocation() override;

public:
	PlayerClass();
	virtual ~PlayerClass() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = L"") override;
	virtual void Update(float DeltaTime) override;
	virtual void Render(LPD3DXSPRITE Sprite) override;
	virtual void CollisionEventBeginByOtherActor(Actor*) override;

};