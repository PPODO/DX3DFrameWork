#include "FirstInGameStage.h"
#include "EnemyClass.h"
#include "ObstacleClass.h"
#include "ObjectPoolClass.h"
#include "BackGroundClass.h"
#include "PlayerClass.h"
#include "SystemClass.h"
#include "EventClass.h"

FirstInGameStage::FirstInGameStage(ObjectPoolClass * OP, PlayerClass * Player) : InGameStage(OP, Player) {
	m_FileSrc.push_back(std::make_pair(L"Stage/BackGround1.png", L"Stage/BackGround1.png"));
	m_FileSrc.push_back(std::make_pair(L"Stage/Ground.png", L"Stage/Ground.png"));

}

FirstInGameStage::~FirstInGameStage() {
}

bool FirstInGameStage::Init(LPDIRECT3DDEVICE9 Device) {
	InGameStage::Init(Device);

	for (size_t i = 0; i < m_FileSrc.size(); i++) {
		D3DXVECTOR3 FirstLocation, SecondLocation;
		std::pair<BackGroundClass*, BackGroundClass*> BackGroundImage(new BackGroundClass, new BackGroundClass);
		if (!BackGroundImage.first || !BackGroundImage.first->Init(Device, m_FileSrc[i].first) || !BackGroundImage.second || !BackGroundImage.second->Init(Device, m_FileSrc[i].second)) {
			return false;
		}
		switch (i) {
		case EBGT_BACKGROUND:
			FirstLocation = D3DXVECTOR3(0.f, BackGroundImage.first->GetActorImage()->GetImageCenter().y - 100, 0.f);
			SecondLocation = D3DXVECTOR3(FLOAT(BackGroundImage.second->GetActorImage()->GetRect().right), 0.f, 0.f) + FirstLocation;
			break;
		case EBGT_GROUND:
			BackGroundImage.first->SetActorCollisionType(ECT_ALLBLOCK);
			BackGroundImage.second->SetActorCollisionType(ECT_ALLBLOCK);
			EventClass::GetInst()->BindCollisionEvent(BackGroundImage.first);
			EventClass::GetInst()->BindCollisionEvent(BackGroundImage.second);
			FirstLocation = D3DXVECTOR3(BackGroundImage.first->GetActorImage()->GetImageCenter().x, GetWindowSize().bottom - BackGroundImage.first->GetActorImage()->GetImageCenter().y, 0.f);
			SecondLocation = D3DXVECTOR3(FLOAT(BackGroundImage.second->GetActorImage()->GetRect().right), 0.f, 0.f) + FirstLocation;
			break;
		}
		BackGroundImage.first->GetActorImage()->SetPosition(FirstLocation);
		BackGroundImage.second->GetActorImage()->SetPosition(SecondLocation);
		BackGroundImage.first->SetOtherBackGroundActor(BackGroundImage.second);
		BackGroundImage.second->SetOtherBackGroundActor(BackGroundImage.first);
		m_BackGroundImages.push_back(BackGroundImage);
	}
	return true;
}

void FirstInGameStage::Update(float DeltaTime) {
	InGameStage::Update(DeltaTime);
}

void FirstInGameStage::Render(LPD3DXSPRITE Sprite) {
	InGameStage::Render(Sprite);

}

void FirstInGameStage::ChangeStageNotification() {
	InGameStage::ChangeStageNotification();

	if (GetPlayerClass()) {
		D3DXVECTOR3 Location = m_BackGroundImages[EBGT_GROUND].first->GetActorImage()->GetPosition() - m_BackGroundImages[EBGT_GROUND].first->GetActorImage()->GetImageCenter();
		GetPlayerClass()->GetActorImage()->SetPosition(D3DXVECTOR3(0.f, Location.y, 0.f) - GetPlayerClass()->GetActorImage()->GetImageCenter());
	}
	EventClass::GetInst()->WakeUpEventThread();
}

void FirstInGameStage::ReleaseStageNotification() {
	InGameStage::ReleaseStageNotification();

}

void FirstInGameStage::PickUpObject() {

}