#pragma once
#include "Actor.h"

class TextClass : public Actor {
private:


public:
	TextClass();

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;
	virtual void Update(float DeltaTime) override;

};