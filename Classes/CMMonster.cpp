#include "CMMonster.h"

/************************************************************************/
/* 怪物基类
/************************************************************************/

bool CMMonsterBasic::init( CCPoint ptMonsterPos,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pMsgRecver )
{
	do 
	{
		CC_BREAK_IF(!CCNode::init());

		m_pMario = pMario;
		//m_pGameMap = pGameMap;
		//注册Update函数
		this->schedule(schedule_selector(CMMonsterBasic::OnCallPerFrame));

		return true;
	} while (false);
	CCLog("fun CMMonsterBasic::init Error!");
	return false;
}

bool CMMonsterBasic::OnCollisionMario()
{
	do 
	{

		return true;
	} while (false);
	CCLog("fun CMMonsterBasic::OnCollisionMario Error!");
	return false;
}

void CMMonsterBasic::Dead( enumMonsterDeadType DeadType )
{
	do 
	{

		return;
	} while (false);
	CCLog("fun CMMonsterBasic::Dead Error!");
}

void CMMonsterBasic::OnCallPerFrame(float fT)
{
	do 
	{
		OnCollisionMario();
		return;
	} while (false);
	CCLog("fun CMMonsterBasic::OnCallPerFrame Error!");
}

/************************************************************************/
/* 蘑菇怪                                                                     */
/************************************************************************/
CMMonsterMushrooms * CMMonsterMushrooms::CreateMonsterMushrooms( CCPoint ptMonsterPos,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pReceiver )
{
	do 
	{
		CMMonsterMushrooms *pMonsterMushrooms = new CMMonsterMushrooms();
		if (pMonsterMushrooms && pMonsterMushrooms->init(ptMonsterPos,pMario,pGameMap,pReceiver))
		{
			pMonsterMushrooms->autorelease();
			return pMonsterMushrooms;
		}
		CC_SAFE_DELETE(pMonsterMushrooms);
		return NULL;
	} while (false);
	CCLog("fun CMHero::create() Error!");
	return NULL;
}

bool CMMonsterMushrooms::init( CCPoint ptMonsterPos,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pReceiver )
{
	do 
	{
		CC_BREAK_IF(!CMMonsterBasic::init(ptMonsterPos,pMario,pGameMap,pReceiver));

		CCSprite* pMushrooms = CCSprite::create("Mushroom0.png");
		CC_BREAK_IF(pMushrooms==NULL);
		pMushrooms->setPosition(ccp(0,0));
		addChild(pMushrooms,enZOrderFront,enTagMainImage);

		setContentSize(pMushrooms->boundingBox().size);

		m_pReceiver = pReceiver;

		return true;
	} while (false);
	CCLog("fun CMMonsterMushrooms::init Error!");
	return false;
}

bool CMMonsterMushrooms::OnCollisionMario()
{
	do 
	{
		CCSprite* pMushrooms = dynamic_cast<CCSprite*>(getChildByTag(enTagMainImage));
		CC_BREAK_IF(pMushrooms==NULL);

		//判断马里奥与蘑菇怪的碰撞
		if (m_pMario->boundingBox().intersectsRect(boundingBox()))
		{
			CCLog("Mashrooms bound");
// 			MsgForCoinCollision* pData = new MsgForCoinCollision;
// 			pData->pCoin = this;
// 			SendMsg(enMsgCoinCollision,pData,sizeof(pCoin));
		}

		return true;
	} while (false);
	CCLog("fun CMMonsterMushrooms::OnCollisionMario Error!");
	return false;
}
