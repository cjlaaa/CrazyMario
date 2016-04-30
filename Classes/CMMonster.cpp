#include "CMMonster.h"

/************************************************************************/
/* 怪物基类
/************************************************************************/
bool CMMonsterBasic::init( CCPoint ptMonsterPos,CMMario *pMario,CMGameMap *pGameMap,CMReceiver *pMsgReceiver )
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
		m_pReceiver = pMsgReceiver;
		m_bHitByFireBall = false;

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

bool CMMonsterBasic::OnCallPerFrame(float fT)
{
	do 
	{
		//判断当怪物离开地图则发消息删除自己
		if (getPositionX()<0 || getPositionY()<0)
		{
			TCmd_Remove_Monster* pData = new TCmd_Remove_Monster;
			pData->pMonster = this;
			SendMsg(enMsgMonsterDisappear,pData,sizeof(pData));
			return true;//需要删除自己
		}

		CC_BREAK_IF(m_pMario==NULL);

		//判断马里奥与当前怪物的距离，用以激活。
		if (abs(m_pMario->getPositionX() - getPositionX())<MONSTER_ACTIVE_DISTANCE)
		{
			m_bIsActivation = true;
		}

		return false;//不需要删除自己
	} while (false);
	CCLog("fun CMMonsterBasic::OnCallPerFrame Error!");
	return false;
}

void CMMonsterBasic::MonsterTurn()
{
	do 
	{
		CCNode *pMonsterRoot = dynamic_cast<CCNode*>(getChildByTag(enTagMainNode));
		CC_BREAK_IF(pMonsterRoot==NULL);
		
		//遍历所有精灵的子节点 使其翻转
		CCArray *pArrayChild = pMonsterRoot->getChildren();
		CC_BREAK_IF(pArrayChild==NULL);

		CCObject *pItem=NULL;
		CCARRAY_FOREACH(pArrayChild,pItem)
		{
			CCSprite *pSprite = dynamic_cast<CCSprite*>(pItem);
			if(pSprite==NULL)continue;
			pSprite->setFlipX(true);
		}
		return;
	} while (false);
	CCLOG("CMMario::MarioTurn Run Error!");
}

void CMMonsterBasic::SetDeadByFireBall()
{
	m_bHitByFireBall = true;
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
		
		//ccbi读取
		//构造一个ccbi文件读取器
		CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader * pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
		//读取动画文件
		CCNode *pCcbiNode = pCcbReader->readNodeGraphFromFile("ccbResources/monster.ccbi", this);
		CC_BREAK_IF(pCcbiNode==NULL);
		pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed("mushroom_run");	
		//读完之后，立刻释放即可
		pCcbReader->release();
		CC_BREAK_IF(pCcbiNode==NULL);
		pCcbiNode->setAnchorPoint(ccp(0,0));
		addChild(pCcbiNode,enZOrderBack,enTagMainNode);
		pCcbiNode->setPosition(ccp(8,8));
		setContentSize(MONSTER_CCSIZE);

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
		CCNode* pMushrooms = dynamic_cast<CCNode*>(getChildByTag(enTagMainNode));
		CC_BREAK_IF(pMushrooms==NULL);

		//马里奥与蘑菇怪的碰撞
		if (m_pMario->boundingBox().intersectsRect(boundingBox()) && m_bIsTouched==false)
		{
			//被踩死
			if (getPositionY()<m_pMario->getPositionY() && abs(m_pMario->getPositionY()-getPositionY())>boundingBox().size.height*0.8)
			{
				//移除行走动画
				removeChildByTag(enTagMainNode);
				//ccbi读取
				//构造一个ccbi文件读取器
				CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
				cocos2d::extension::CCBReader * pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
				//读取动画文件
				CCNode *pCcbiNode = pCcbReader->readNodeGraphFromFile("ccbResources/monster.ccbi", this);
				CC_BREAK_IF(pCcbiNode==NULL);
				pCcbReader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(CMMonsterMushrooms::OnCallDeadAnimationFinished));
				pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed("mushroom_die");	
				//读完之后，立刻释放即可
				pCcbReader->release();
				CC_BREAK_IF(pCcbiNode==NULL);
				addChild(pCcbiNode,enZOrderBack,enTagMainNode);
				pCcbiNode->setPosition(ccp(8,8));
				
				return true;
			}
			else
			{
				TCmd_Remove_Monster* pData = new TCmd_Remove_Monster;
				pData->pMonster = this;
				SendMsg(enMsgBeHurt,pData,sizeof(pData));

				return false;
			}
			m_bIsTouched = true;
		}

		return false;
	} while (false);
	CCLog("fun CMMonsterMushrooms::OnCollisionMario Error!");
	return false;
}

void CMMonsterMushrooms::OnCallDeadAnimationFinished()
{
	//发送怪物移除消息
	TCmd_Remove_Monster* pData = new TCmd_Remove_Monster;
	pData->pMonster = this;
	SendMsg(enMsgMushroomsStamp,pData,sizeof(pData));
}

bool CMMonsterMushrooms::OnCallPerFrame( float fT )
{
	do 
	{
		//被火球打死
		if (m_bHitByFireBall==true)
		{
			m_bHitByFireBall = false;
			removeChildByTag(enTagMainNode);
			//ccbi读取
			//构造一个ccbi文件读取器
			CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			cocos2d::extension::CCBReader * pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
			//读取动画文件
			CCNode *pFireBall = pCcbReader->readNodeGraphFromFile("ccbResources/monster.ccbi", this);
			CC_BREAK_IF(pFireBall==NULL);
			pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed("mushroom_die");	
			pCcbReader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(CMMonsterMushrooms::OnCallDeadAnimationFinished));
			//读完之后，立刻释放即可
			pCcbReader->release();
			CC_BREAK_IF(pFireBall==NULL);
			pFireBall->setAnchorPoint(ccp(0,0));
			addChild(pFireBall,enZOrderFront,enTagMainNode);
			pFireBall->setPosition(ccp(0,0));
			return true;
		}

		//是否激活
		if (m_bIsActivation==true)
		{
			//移动与碰撞
			if (m_MoveDirection == enMoveLeft)
			{
				//用怪物左方的2个瓦片来判断移动碰撞
				CCSprite* pTileSpriteLeftTop = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX(),getPositionY()+getContentSize().height));
				CCSprite* pTileSpriteLeftMid = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX(),getPositionY()+getContentSize().height/2));
				if (pTileSpriteLeftTop!=NULL || pTileSpriteLeftMid!=NULL)
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
				CCSprite* pTileSpriteRightTop = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+getContentSize().width,getPositionY()+getContentSize().height));
				CCSprite* pTileSpriteRightMid = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+getContentSize().width,getPositionY()+getContentSize().height/2));
				if (pTileSpriteRightTop!=NULL || pTileSpriteRightMid!=NULL)
				{
					m_MoveDirection = enMoveLeft;
				}
				else
				{
					setPositionX(getPositionX()+1);
				}
			}

			//用怪物下方的三个瓦片来判断掉落碰撞
			CCSprite* pTileSpriteBottomMid = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+boundingBox().size.width/2,getPositionY()));
			CCSprite* pTileSpriteBottomLeft = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+COLLISION_POS_ADJUSTMENT,getPositionY()));
			CCSprite* pTileSpriteBottomRight = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+boundingBox().size.width-COLLISION_POS_ADJUSTMENT,getPositionY()));
			if (pTileSpriteBottomLeft!=NULL || pTileSpriteBottomMid!=NULL || pTileSpriteBottomRight!=NULL)
			{
				//掉落速度归零
				m_fDropSpeedPlus = 0;

				//防止下落速度过快导致陷进地里
				if (pTileSpriteBottomMid!=NULL)
				{
					if (getPositionY()<pTileSpriteBottomMid->getPositionY()+pTileSpriteBottomMid->getContentSize().height)
					{
						setPositionY(getPositionY()+abs(getPositionY()-(pTileSpriteBottomMid->getPositionY()+pTileSpriteBottomMid->getContentSize().height))-0.1);
					}
				}
			}
			else
			{
				setPositionY(getPositionY()-m_fDropSpeedPlus);
				//掉落加速度
				m_fDropSpeedPlus += DROP_SPEED_PLUS;
			}
		}

		

		return (CMMonsterBasic::OnCallPerFrame(fT)||OnCollisionMario());
	} while (false);
	
	CCLog("fun CMMonsterMushrooms::OnCallPerFrame Error!");

	return false;
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

		//ccbi读取
		//构造一个ccbi文件读取器
		CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader * pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
		//读取动画文件
		CCNode *pCcbiNode = pCcbReader->readNodeGraphFromFile("ccbResources/monster.ccbi", this);
		CC_BREAK_IF(pCcbiNode==NULL);
		pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed("tortoise_run");	
		//读完之后，立刻释放即可
		pCcbReader->release();
		CC_BREAK_IF(pCcbiNode==NULL);
		pCcbiNode->setAnchorPoint(ccp(0,0));
		addChild(pCcbiNode,enZOrderBack,enTagMainNode);
		pCcbiNode->setPositionY(12);
		setContentSize(MONSTER_CCSIZE);

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
		CCNode* pTortoise = dynamic_cast<CCNode*>(getChildByTag(enTagMainNode));
		CC_BREAK_IF(pTortoise==NULL);

		//马里奥与怪的碰撞
		if (m_pMario->boundingBox().intersectsRect(boundingBox()))
		{
			//被踩
			if (getPositionY()<m_pMario->getPositionY() && abs(m_pMario->getPositionY()-getPositionY())>boundingBox().size.height*0.8)
			{
				removeChildByTag(enTagMainNode);
				//ccbi读取
				//构造一个ccbi文件读取器
				CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
				cocos2d::extension::CCBReader * pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
				//读取动画文件
				CCNode *pFireBall = pCcbReader->readNodeGraphFromFile("ccbResources/monster.ccbi", this);
				CC_BREAK_IF(pFireBall==NULL);
				pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed("tortoise_die");	
				pCcbReader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(CMMonsterTortoise::OnCallDeadAnimationFinished));
				//读完之后，立刻释放即可
				pCcbReader->release();
				CC_BREAK_IF(pFireBall==NULL);
				pFireBall->setAnchorPoint(ccp(0,0));
				addChild(pFireBall,enZOrderFront,enTagMainNode);
				pFireBall->setPosition(ccp(0,0));

				m_bIsTouched = true;

				return true;
			}
			else
			{
				TCmd_Remove_Monster* pData = new TCmd_Remove_Monster;
				pData->pMonster = this;
				SendMsg(enMsgBeHurt,pData,sizeof(pData));

				return false;
			}
		}

		return false;
	} while (false);
	CCLog("fun CMMonsterTortoise::OnCollisionMario Error!");
	return false;
}

bool CMMonsterTortoise::OnCallPerFrame( float fT )
{
	do 
	{
		//被火球打死
		if (m_bHitByFireBall==true)
		{
			m_bHitByFireBall = false;
			removeChildByTag(enTagMainNode);
			//ccbi读取
			//构造一个ccbi文件读取器
			CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			cocos2d::extension::CCBReader * pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
			//读取动画文件
			CCNode *pFireBall = pCcbReader->readNodeGraphFromFile("ccbResources/monster.ccbi", this);
			CC_BREAK_IF(pFireBall==NULL);
			pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed("tortoise_die");	
			pCcbReader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(CMMonsterTortoise::OnCallDeadAnimationFinished));
			//读完之后，立刻释放即可
			pCcbReader->release();
			CC_BREAK_IF(pFireBall==NULL);
			pFireBall->setAnchorPoint(ccp(0,0));
			addChild(pFireBall,enZOrderFront,enTagMainNode);
			pFireBall->setPosition(ccp(0,0));
			return true;
		}

		CCNode* pTortoise = dynamic_cast<CCNode*>(getChildByTag(enTagMainNode));
		CC_BREAK_IF(pTortoise==NULL);

		//是否激活
		if (m_bIsActivation==true)
		{
			//移动与碰撞
			if (m_MoveDirection == enMoveLeft)
			{
				//用怪物左方的2个瓦片来判断移动碰撞
				CCSprite* pTileSpriteLeftTop = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX(),getPositionY()+getContentSize().height));
				CCSprite* pTileSpriteLeftMid = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX(),getPositionY()+getContentSize().height/2));
				if (pTileSpriteLeftTop!=NULL || pTileSpriteLeftMid!=NULL)
				{
					m_MoveDirection = enMoveRight;
					MonsterTurn();
				}
				else
				{
					setPositionX(getPositionX()-1);
				}
			}
			else if(m_MoveDirection == enMoveRight)
			{
				//用怪物右方的2个瓦片来判断移动碰撞
				CCSprite* pTileSpriteRightTop = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+getContentSize().width,getPositionY()+getContentSize().height));
				CCSprite* pTileSpriteRightMid = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+getContentSize().width,getPositionY()+getContentSize().height/2));
				if (pTileSpriteRightTop!=NULL || pTileSpriteRightMid!=NULL)
				{
					m_MoveDirection = enMoveLeft;
					MonsterTurn();
				}
				else
				{
					setPositionX(getPositionX()+1);
				}
			}

			//用怪物下方的三个瓦片来判断掉落碰撞
			CCSprite* pTileSpriteBottomMid = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+boundingBox().size.width/2,getPositionY()));
			CCSprite* pTileSpriteBottomLeft = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+COLLISION_POS_ADJUSTMENT,getPositionY()));
			CCSprite* pTileSpriteBottomRight = m_pGameMap->TileMapLayerPosToTileSprite(ccp(getPositionX()+boundingBox().size.width-COLLISION_POS_ADJUSTMENT,getPositionY()));
			if (pTileSpriteBottomMid!=NULL || pTileSpriteBottomLeft!=NULL || pTileSpriteBottomRight!=NULL)
			{
				//掉落速度归零
				m_fDropSpeedPlus = 0;

				//防止下落速度过快导致陷进地里
				if (pTileSpriteBottomMid!=NULL)
				{
					if (getPositionY()<pTileSpriteBottomMid->getPositionY()+pTileSpriteBottomMid->getContentSize().height)
					{
						setPositionY(getPositionY()+abs(getPositionY()-(pTileSpriteBottomMid->getPositionY()+pTileSpriteBottomMid->getContentSize().height))-0.1);
					}
				}
			}
			else
			{
				setPositionY(getPositionY()-m_fDropSpeedPlus);
				//掉落加速度
				m_fDropSpeedPlus += DROP_SPEED_PLUS;
			}
		}

		return (CMMonsterBasic::OnCallPerFrame(fT)||OnCollisionMario());
	} while (false);
	CCLog("fun CMMonsterTortoise::OnCallPerFrame Error!");
	return false;
}

void CMMonsterTortoise::OnCallDeadAnimationFinished()
{
	//发送怪物移除消息
	TCmd_Remove_Monster* pData = new TCmd_Remove_Monster;
	pData->pMonster = this;
	SendMsg(enMsgTortoiseStamp,pData,sizeof(pData));
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

		//ccbi读取
		//构造一个ccbi文件读取器
		CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
		cocos2d::extension::CCBReader * pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
		//读取动画文件
		CCNode *pCcbiNode = pCcbReader->readNodeGraphFromFile("ccbResources/monster.ccbi", this);
		CC_BREAK_IF(pCcbiNode==NULL);
		pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed("flower_up");	
		//读完之后，立刻释放即可
		pCcbReader->release();
		CC_BREAK_IF(pCcbiNode==NULL);
		pCcbiNode->setAnchorPoint(ccp(0,0));
		addChild(pCcbiNode,enZOrderBack,enTagMainNode);
		pCcbiNode->setPositionY(8);
		setContentSize(MONSTER_CCSIZE);

		m_bIsTouched = false;
		m_fMoveCount = 40;
		m_MoveDirection = enMoveLeft;
		return true;
	} while (false);
	CCLog("fun CMMonsterFlower::init Error!");
	return false;
}

bool CMMonsterFlower::OnCollisionMario()
{
	do 
	{
		CCNode* pFlower = dynamic_cast<CCNode*>(getChildByTag(enTagMainNode));
		CC_BREAK_IF(pFlower==NULL);

		//马里奥与怪的碰撞
		if (m_pMario->boundingBox().intersectsRect(boundingBox()))
		{
			TCmd_Remove_Monster* pData = new TCmd_Remove_Monster;
			pData->pMonster = this;
			SendMsg(enMsgBeHurt,pData,sizeof(pData));

			return true;
		}

		return false;
	} while (false);
	CCLog("fun CMMonsterFlower::OnCollisionMario Error!");
	return false;
}

bool CMMonsterFlower::OnCallPerFrame( float fT )
{
	do 
	{
		//被火球打死
		if (m_bHitByFireBall==true)
		{
			m_bHitByFireBall = false;
			removeChildByTag(enTagMainNode);
			//ccbi读取
			//构造一个ccbi文件读取器
			CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			cocos2d::extension::CCBReader * pCcbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
			//读取动画文件
			CCNode *pFireBall = pCcbReader->readNodeGraphFromFile("ccbResources/monster.ccbi", this);
			CC_BREAK_IF(pFireBall==NULL);
			pCcbReader->getAnimationManager()->runAnimationsForSequenceNamed("tortoise_die");	
			pCcbReader->getAnimationManager()->setAnimationCompletedCallback(this,callfunc_selector(CMMonsterFlower::OnCallDeadAnimationFinished));
			//读完之后，立刻释放即可
			pCcbReader->release();
			CC_BREAK_IF(pFireBall==NULL);
			pFireBall->setAnchorPoint(ccp(0,0));
			addChild(pFireBall,enZOrderFront,enTagMainNode);
			pFireBall->setPosition(ccp(0,0));
			return true;
		}

		CCNode* pTortoise = dynamic_cast<CCNode*>(getChildByTag(enTagMainNode));
		CC_BREAK_IF(pTortoise==NULL);

		//判断马里奥与当前怪物的距离，用以激活。
		if (abs(m_pMario->getPositionX() - getPositionX())<MONSTER_ACTIVE_DISTANCE)
		{
			m_bIsActivation = true;
		}

		//是否激活
		if (m_bIsActivation==false)
		{
			return false;
		}

		//花的反复运动
		if(getActionByTag(enTagActionRepeat)==NULL)
		{
			CCMoveBy* pMoveByDown = CCMoveBy::create(1,ccp(0,-30));
			CCDelayTime* pDelayTime = CCDelayTime::create(1);
			CCMoveBy* pMoveByUp = CCMoveBy::create(1,ccp(0,30));

			CCRepeatForever* pRepeatForever = CCRepeatForever::create(CCSequence::create(pMoveByDown,pDelayTime,pMoveByUp,pDelayTime, NULL));
			pRepeatForever->setTag(enTagActionRepeat);
			runAction(pRepeatForever);
		}

		return (CMMonsterBasic::OnCallPerFrame(fT)||OnCollisionMario());
	} while (false);
	CCLog("fun CMMonsterFlower::OnCallPerFrame Error!");
	return false;
}

void CMMonsterFlower::OnCallDeadAnimationFinished()
{
	//发送怪物移除消息
	TCmd_Remove_Monster* pData = new TCmd_Remove_Monster;
	pData->pMonster = this;
	SendMsg(enMsgTortoiseStamp,pData,sizeof(pData));
}