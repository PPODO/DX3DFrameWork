#pragma once
#include "StageClass.h"
#include <stack>
#include <vector>
#include <utility>
#include <map>

class InGameStage : public StageClass {
	enum EBackGroundType { EBGT_BACKGROUND, EBGT_BACKGROUND2, EBGT_BACKGROUND3, EBGT_GROUND };
private:
	class ObjectPoolClass* m_PoolManager;
	class PlayerClass* m_Player;

private:
	std::vector<std::pair<LPCTSTR, LPCTSTR>> m_FileSrc;
	std::vector<std::pair<class BackGroundClass*, class BackGroundClass*>> m_BackGroundImages;

	std::map<std::string, std::vector<std::pair<std::string, size_t>>> m_PoolObjectType;
	std::vector<std::vector<std::stack<class Actor*>>> m_PoolObjects;

private:
	void UpdatePoolObjects();

public:
	InGameStage(class ObjectPoolClass* OP, class PlayerClass* Player);
	virtual ~InGameStage() override;

public:
	virtual bool Init(LPDIRECT3DDEVICE9 Device);
	virtual void Update(float DeltaTime);
	virtual void Render(LPD3DXSPRITE Sprite);
	virtual void ChangeStageNotification() override;
	virtual void ReleaseStageNotification() override;
};