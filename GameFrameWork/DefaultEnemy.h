#pragma once
#include "EnemyClass.h"

class DefaultEnemy : public EnemyClass {
private:


protected:
	virtual void ProcessEnemyMovement() override;

public:
	DefaultEnemy();

	virtual bool Init(LPDIRECT3DDEVICE9 Device, LPCTSTR FileSrc = nullptr, RECT CustomRect = { -1 }) override;

};