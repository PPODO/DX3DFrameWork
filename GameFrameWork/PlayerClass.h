#pragma once
#include "Actor.h"

class PlayerClass : public Actor {
private:


public:
	PlayerClass(std::vector<Actor*>&);

	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc, RECT CustomRect) override;
	virtual void Update(float DeltaTime) override;
	virtual void Destroy() override;
};

