#include "CMMonster.h"

/************************************************************************/
/* 怪物基类
/************************************************************************/
bool CMMonsterBasic::init( CCPoint ptMonsterPos,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pMsgRecver )
{
	do 
	{
		CC_BREAK_IF(!CCNode::init());

		//初始化成员变量
		m_pMario = pMario;
		m_pGameMap = pGameMap;
		m_bIsActivation = false;
		m_MoveDirection = enMoveLeft;
		m_fDropSpeedPlus = 0;

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

void CMMonsterBasic::Dead( enMonsterDeadType DeadType )
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
		//判断当怪物离开地图则发消息删除自己
		if (getPositionX()<0 || getPositionY()<0)
		{
			MsgForMonsterDisappear* pData = new MsgForMonsterDisappear;
			pData->pMonster = this;
			SendMsg(enMsgMonsterDisappear,pData,sizeof(pData));
		}

		//判断马里奥与当前怪物的距离，用以激活。
		if (abs(m_pMario->getPositionX() - getPositionX())<MONSTER_ACTIVE_DISTANCE)
		{
			m_bIsActivation = true;
		}

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
		pMushrooms->setAnchorPoint(ccp(0,0));
		addChild(pMushrooms,enZOrderFront,enTagMainSprite);

		setContentSize(pMushrooms->boundingBox().size);

		m_pReceiver = pReceiver;
		m_bIsTouched = false;

		return true;
	} while (false);
	CCLog("fun CMMonsterMushrooms::init Error!");
	return false;
}

bool CMMonsterMushrooms::OnCollisionMario()
{
	do 
	{
		CCSprite* pMushrooms = dynamic_cast<CCSprite*>(getChildByTag(enTagMainSprite));
		CC_BREAK_IF(pMushrooms==NULL);

		//马里奥与蘑菇怪的碰撞
		if (m_pMario->boundingBox().intersectsRect(boundingBox()))
		{
			//被踩死
			if (getPositionY()<m_pMario->getPositionY() && abs(m_pMario->getPositionY()-getPositionY())>boundingBox().size.height*0.8)
			{
				MsgForMonsterDisappear* pData = new MsgForMonsterDisappear;
				pData->pMonster = this;
				SendMsg(enMsgStamp,pData,sizeof(pData));

				m_bIsTouched = true;
			}
			else
			{
				MsgForMonsterDisappear* pData = new MsgForMonsterDisappear;
				pData->pMonster = this;
				SendMsg(enMsgBeHurt,pData,sizeof(pData));
			}
		}

		return true;
	} while (false);
	CCLog("fun CMMonsterMushrooms::OnCollisionMario Error!");
	return false;
}

void CMMonsterMushrooms::OnCallPerFrame( float fT )
{
	do 
	{
		CMMonsterBasic::OnCallPerFrame(fT);
		OnCollisionMario();

		//是否激活
		if (m_bIsActivation==false)
		{
			return;
		}

		CCSprite* pTileSprite1 = NULL;
		CCSprite* pTileSprite2 = NULL;
		CCSprite* pTileSprite3 = NULL;
		//移动与碰撞
		if (m_MoveDirection == enMoveLeft)
		{
			//用怪物左方的2个瓦片来判断移动碰撞
			pTileSprite1 = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX(),getPositionY()+getContentSize().height));
			pTileSprite2 = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX(),getPositionY()+getContentSize().height/2));
			if (pTileSprite1!=NULL || pTileSprite2!=NULL)
			{
				m_MoveDirection = enMoveRight;
			}
			else
			{
				setPositionX(getPositionX()-1);
			}
		}
		else if(m_MoveDirection == enMoveRight)
		{
			//用怪物右方的2个瓦片来判断移动碰撞
			pTileSprite1 = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+getContentSize().width,getPositionY()+getContentSize().height));
			pTileSprite2 = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+getContentSize().width,getPositionY()+getContentSize().height/2));
			if (pTileSprite1!=NULL || pTileSprite2!=NULL)
			{
				m_MoveDirection = enMoveLeft;
			}
			else
			{
				setPositionX(getPositionX()+1);
			}
		}

		pTileSprite1 = NULL;
		pTileSprite2 = NULL;
		pTileSprite3 = NULL;
		//用怪物下方的三个瓦片来判断掉落碰撞
		pTileSprite1 = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+boundingBox().size.width/2,getPositionY()));
		pTileSprite2 = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+COLLISION_POS_ADJUSTMENT,getPositionY()));
		pTileSprite3 = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+boundingBox().size.width-COLLISION_POS_ADJUSTMENT,getPositionY()));
		if (pTileSprite1!=NULL || pTileSprite2!=NULL || pTileSprite3!=NULL)
		{
			//掉落速度归零
			m_fDropSpeedPlus = 0;
		}
		else
		{
			setPositionY(getPositionY()-m_fDropSpeedPlus);
			//掉落加速度
			m_fDropSpeedPlus += DROP_SPEED_PLUS;
		}

		return;
	} while (false);
	CCLog("fun CMMonsterMushrooms::OnCallPerFrame Error!");
}
/************************************************************************/
/* 乌龟                                                               */
/************************************************************************/
CMMonsterTortoise * CMMonsterTortoise::CreateMonsterTortoise( CCPoint ptMonsterPos,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pReceiver )
{
	do 
	{
		CMMonsterTortoise *pMonsterTortoise = new CMMonsterTortoise();
		if (pMonsterTortoise && pMonsterTortoise->init(ptMonsterPos,pMario,pGameMap,pReceiver))
		{
			pMonsterTortoise->autorelease();
			return pMonsterTortoise;
		}
		CC_SAFE_DELETE(pMonsterTortoise);
		return NULL;
	} while (false);
	CCLog("fun CMMonsterTortoise::CreateMonsterTortoise Error!");
	return NULL;
}

bool CMMonsterTortoise::init( CCPoint ptMonsterPos,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pReceiver )
{
	do 
	{
		CC_BREAK_IF(!CMMonsterBasic::init(ptMonsterPos,pMario,pGameMap,pReceiver));

		CCSprite* pTortoise = CCSprite::create("tortoise_left.png");
		CC_BREAK_IF(pTortoise==NULL);
		pTortoise->setAnchorPoint(ccp(0,0));
		addChild(pTortoise,enZOrderFront,enTagMainSprite);

		setContentSize(pTortoise->boundingBox().size);

		m_pReceiver = pReceiver;
		m_bIsTouched = false;

		return true;
	} while (false);
	CCLog("fun CMMonsterMushrooms::init Error!");
	return false;
}

bool CMMonsterTortoise::OnCollisionMario()
{
	do 
	{
		CCSprite* pTortoise = dynamic_cast<CCSprite*>(getChildByTag(enTagMainSprite));
		CC_BREAK_IF(pTortoise==NULL);

		//马里奥与怪的碰撞
		if (m_pMario->boundingBox().intersectsRect(boundingBox()))
		{
			//被踩
			if (getPositionY()<m_pMario->getPositionY() && abs(m_pMario->getPositionY()-getPositionY())>boundingBox().size.height*0.8)
			{
// 				MsgForMonsterDisappear* pData = new MsgForMonsterDisappear;
// 				pData->pMonster = this;
// 				SendMsg(enMsgStamp,pData,sizeof(pData));

				pTortoise->setTexture(CCTextureCache::sharedTextureCache()->addImage("tortoise0.png"));

				m_bIsTouched = true;
			}
			else
			{
				MsgForMonsterDisappear* pData = new MsgForMonsterDisappear;
				pData->pMonster = this;
				SendMsg(enMsgBeHurt,pData,sizeof(pData));
			}
		}

		return true;
	} while (false);
	CCLog("fun CMMonsterTortoise::OnCollisionMario Error!");
	return false;
}

void CMMonsterTortoise::OnCallPerFrame( float fT )
{
	do 
	{
		CCSprite* pTortoise = dynamic_cast<CCSprite*>(getChildByTag(enTagMainSprite));
		CC_BREAK_IF(pTortoise==NULL);

		CMMonsterBasic::OnCallPerFrame(fT);
		OnCollisionMario();

		//是否激活
		if (m_bIsActivation==false)
		{
			return;
		}

		CCSprite* pTileSprite1 = NULL;
		CCSprite* pTileSprite2 = NULL;
		CCSprite* pTileSprite3 = NULL;
		//移动与碰撞
		if (m_MoveDirection == enMoveLeft)
		{
			//用怪物左方的2个瓦片来判断移动碰撞
			pTileSprite1 = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX(),getPositionY()+getContentSize().height));
			pTileSprite2 = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX(),getPositionY()+getContentSize().height/2));
			if (pTileSprite1!=NULL || pTileSprite2!=NULL)
			{
				m_MoveDirection = enMoveRight;
				pTortoise->setTexture(CCTextureCache::sharedTextureCache()->addImage("tortoise_right.png"));
			}
			else
			{
				setPositionX(getPositionX()-1);
			}
		}
		else if(m_MoveDirection == enMoveRight)
		{
			//用怪物右方的2个瓦片来判断移动碰撞
			pTileSprite1 = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+getContentSize().width,getPositionY()+getContentSize().height));
			pTileSprite2 = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+getContentSize().width,getPositionY()+getContentSize().height/2));
			if (pTileSprite1!=NULL || pTileSprite2!=NULL)
			{
				m_MoveDirection = enMoveLeft;
				pTortoise->setTexture(CCTextureCache::sharedTextureCache()->addImage("tortoise_left.png"));
			}
			else
			{
				setPositionX(getPositionX()+1);
			}
		}

		pTileSprite1 = NULL;
		pTileSprite2 = NULL;
		pTileSprite3 = NULL;
		//用怪物下方的三个瓦片来判断掉落碰撞
		pTileSprite1 = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+boundingBox().size.width/2,getPositionY()));
		pTileSprite2 = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+COLLISION_POS_ADJUSTMENT,getPositionY()));
		pTileSprite3 = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+boundingBox().size.width-COLLISION_POS_ADJUSTMENT,getPositionY()));
		if (pTileSprite1!=NULL || pTileSprite2!=NULL || pTileSprite3!=NULL)
		{
			//掉落速度归零
			m_fDropSpeedPlus = 0;
		}
		else
		{
			setPositionY(getPositionY()-m_fDropSpeedPlus);
			//掉落加速度
			m_fDropSpeedPlus += DROP_SPEED_PLUS;
		}

		return;
	} while (false);
	CCLog("fun CMMonsterTortoise::OnCallPerFrame Error!");
}

/************************************************************************/
/* 花                                                                    */
/************************************************************************/
CMMonsterFlower * CMMonsterFlower::CreateMonsterFlower( CCPoint ptMonsterPos,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pReceiver )
{
	do 
	{
		CMMonsterFlower *pMonsterFlower = new CMMonsterFlower();
		if (pMonsterFlower && pMonsterFlower->init(ptMonsterPos,pMario,pGameMap,pReceiver))
		{
			pMonsterFlower->autorelease();
			return pMonsterFlower;
		}
		CC_SAFE_DELETE(pMonsterFlower);
		return NULL;
	} while (false);
	CCLog("fun CMMonsterFlower::CreateMonsterFlower Error!");
	return NULL;
}

bool CMMonsterFlower::init( CCPoint ptMonsterPos,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pReceiver )
{
	do 
	{
		CC_BREAK_IF(!CMMonsterBasic::init(ptMonsterPos,pMario,pGameMap,pReceiver));

		CCSprite* pFlower = CCSprite::create("flower0.png");
		CC_BREAK_IF(pFlower==NULL);
		pFlower->setAnchorPoint(ccp(0.5,0));
		addChild(pFlower,enZOrderFront,enTagMainSprite);

		setContentSize(pFlower->boundingBox().size);

		m_pReceiver = pReceiver;
		m_bIsTouched = false;

		return true;
	} while (false);
	CCLog("fun CMMonsterFlower::init Error!");
	return false;
}

bool CMMonsterFlower::OnCollisionMario()
{
	do 
	{
		CCSprite* pFlower = dynamic_cast<CCSprite*>(getChildByTag(enTagMainSprite));
		CC_BREAK_IF(pFlower==NULL);

		//马里奥与怪的碰撞
		if (m_pMario->boundingBox().intersectsRect(boundingBox()))
		{
			MsgForMonsterDisappear* pData = new MsgForMonsterDisappear;
			pData->pMonster = this;
			SendMsg(enMsgBeHurt,pData,sizeof(pData));
		}

		return true;
	} while (false);
	CCLog("fun CMMonsterFlower::OnCollisionMario Error!");
	return false;
}

void CMMonsterFlower::OnCallPerFrame( float fT )
{
	do 
	{
		CCSprite* pTortoise = dynamic_cast<CCSprite*>(getChildByTag(enTagMainSprite));
		CC_BREAK_IF(pTortoise==NULL);

		CMMonsterBasic::OnCallPerFrame(fT);
		OnCollisionMario();

		//是否激活
		if (m_bIsActivation==false)
		{
			return;
		}

		return;
	} while (false);
	CCLog("fun CMMonsterFlower::OnCallPerFrame Error!");
}
